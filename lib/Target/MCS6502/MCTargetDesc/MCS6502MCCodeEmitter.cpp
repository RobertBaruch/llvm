//===-- MCS6502MCCodeEmitter.cpp - Convert MCS6502 code to machine code ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the definitions of the MCS6502MCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MCS6502FixupKinds.h"
#include "MCTargetDesc/MCS6502MCTargetDesc.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");
STATISTIC(MCNumFixups, "Number of MC fixups created");

namespace {

class MCS6502MCCodeEmitter : public MCCodeEmitter {
  const MCInstrInfo &MCII;
  const MCRegisterInfo &MRI;
  MCContext &Ctx;

public:
  MCS6502MCCodeEmitter(const MCInstrInfo &mcii, const MCRegisterInfo &mri,
                       MCContext &ctx)
      : MCII(mcii), MRI(mri), Ctx(ctx) {}

  // Disallow copy and assign.
  MCS6502MCCodeEmitter(const MCS6502MCCodeEmitter &) = delete;
  void operator=(const MCS6502MCCodeEmitter &) = delete;

  ~MCS6502MCCodeEmitter() override = default;

  // Takes the given MCInst and outputs bytes into the output stream, possibly
  // adding fixups to the Fixups vector for bits that need to be fixed up
  // later. The output bytes are called a "fragment".
  //
  // Called by MCAsmStreamer::AddEncodingComment via a long chain ultimately
  // called by MCS6502AsmParser::MtchAndEmitInstruction when it calls
  // Out.EmitInstruction.
  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  /// TableGen'erated function for getting the binary encoding for an
  /// instruction. Defined in MCS6502GenMCCodeEmitter.inc.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// Returns binary encoding of an operand. If the machine operand requires
  /// relocation, records the relocation and returns zero. Called by
  /// getBinaryCodeForInstr.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;
};

void MCS6502MCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  union {
    char Bytes[4];
    uint32_t Bits;
  } Code;

  LLVM_DEBUG(dbgs() << "encodeInstruction: getBinaryCodeForInstr\n");

  // getBinaryCodeForInstr is generated, in MCS6502GenMCCodeEmitter.inc, and
  // it returns 64 bits. For each operand, it calls getMachineOpValue so that
  // it can put those bits in the right place in the intsruction. Bits in the
  // operand can't yet be determined (i.e. they must be fixed up later) are 0,
  // and a Fixup is placed by getMachineOpValue in the Fixups vector.
  Code.Bits = getBinaryCodeForInstr(MI, Fixups, STI);

  // We don't know how big the instruction is in bytes, so we get the
  // description of the instruction from the MCII table.
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());

  switch (Desc.getSize()) {
  case 3:
    support::endian::write(OS, Code.Bytes[2], support::endianness::little);
    LLVM_FALLTHROUGH;
  case 2:
    support::endian::write(OS, Code.Bytes[1], support::endianness::little);
    LLVM_FALLTHROUGH;
  case 1:
    support::endian::write(OS, Code.Bytes[0], support::endianness::little);
    break;
  }
  ++MCNumEmitted;
  LLVM_DEBUG(dbgs() << "encodeInstruction done\n");
}

unsigned
MCS6502MCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());

  LLVM_DEBUG(dbgs() << "encodeInstruction: getMachineOpValue\n");

  // Constants just get their value returned, no fixups needed.
  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  assert(MO.isExpr() &&
         "getMachineOpValue expects only expressions or immediates");

  const MCExpr *Expr = MO.getExpr();
  MCExpr::ExprKind Kind = Expr->getKind();
  MCS6502::Fixups FixupKind = MCS6502::fixup_mcs6502_invalid;

  LLVM_DEBUG(dbgs() << "encodeInstruction: MCExpr kind: " << Kind << "\n");

  // VK = Variant Kind. What are they used for?
  if (Kind == MCExpr::SymbolRef &&
      cast<MCSymbolRefExpr>(Expr)->getKind() == MCSymbolRefExpr::VK_None) {
    if (Desc.isConditionalBranch()) {
      FixupKind = MCS6502::fixup_mcs6502_branch;
    } else {
      FixupKind = Desc.getSize() == 2 ? MCS6502::fixup_mcs6502_symbol8
                                      : MCS6502::fixup_mcs6502_symbol16;
    }
    LLVM_DEBUG(dbgs() << "encodeInstruction: Symbol fixup added\n");
  }

  assert(FixupKind != MCS6502::fixup_mcs6502_invalid &&
         "Unhandled expression!");

  Fixups.push_back(MCFixup::create(0 /* offset */, Expr, MCFixupKind(FixupKind),
                                   MI.getLoc()));
  ++MCNumFixups;
  return 0;
}

} // namespace

MCCodeEmitter *llvm::createMCS6502MCCodeEmitter(const MCInstrInfo &MCII,
                                                const MCRegisterInfo &MRI,
                                                MCContext &Ctx) {
  return new MCS6502MCCodeEmitter(MCII, MRI, Ctx);
}

#include "MCS6502GenMCCodeEmitter.inc"

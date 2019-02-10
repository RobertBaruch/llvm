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
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

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

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  /// TableGen'erated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// Returns binary encoding of an operand. If the machine operand requires
  /// relocation, records the relocation and returns zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;
};

void MCS6502MCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  // TODO: write only the bytes in the instruction!
  union {
    char Bytes[4];
    uint32_t Bits;
  } Code;

  Code.Bits = getBinaryCodeForInstr(MI, Fixups, STI);

  // That's nice, but we still don't know how big the instruction is in bytes,
  // so we get the description of the instruction from the MCII table.

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
}

unsigned
MCS6502MCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  if (MO.isReg()) // in practice, there are no register operands.
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  llvm_unreachable("Unhandled expression!");
  return 0;
}

} // namespace

MCCodeEmitter *llvm::createMCS6502MCCodeEmitter(const MCInstrInfo &MCII,
                                                const MCRegisterInfo &MRI,
                                                MCContext &Ctx) {
  return new MCS6502MCCodeEmitter(MCII, MRI, Ctx);
}

#include "MCS6502GenMCCodeEmitter.inc"

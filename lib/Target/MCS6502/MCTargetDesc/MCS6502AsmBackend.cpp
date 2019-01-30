//===-- MCS6502AsmBackend.cpp - MCS6502 Assembler Backend -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MCS6502FixupKinds.h"
#include "MCTargetDesc/MCS6502MCTargetDesc.h"
#include "llvm/ADT/APInt.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "asmbackend"

using namespace llvm;

namespace {

class MCS6502AsmBackend : public MCAsmBackend {
  uint8_t OSABI;

public:
  MCS6502AsmBackend(uint8_t OSABI)
      : MCAsmBackend(support::endianness::little), OSABI(OSABI) {}
  ~MCS6502AsmBackend() override {}

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    return createMCS6502ELFObjectWriter(OSABI, false /* Is64Bit */);
  }

  // Called by MCAsmBackend::fixupNeedsRelaxationAdvanced via
  // MCAssembler::fixupNeedsRelaxation.
  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override;

  unsigned getNumFixupKinds() const override {
    return MCS6502::NumTargetFixupKinds;
  }

  // Gets the relaxed version of the given Opcode, or the Opcode if there is
  // none. It would be nice to encode this in the Instruction in
  // MCS6502InstrInfo.td, but there doesn't seem to be an easy way to do that.
  unsigned getRelaxedOpcode(unsigned Opcode) const;

  // Called from MCAsmStreamer::AddEncodingComment just after encodeInstruction.
  // In the encoding comment, the bytes to be fixed up are symbolized as
  // a letter (e.g. 'A'), and the letter indicates the fixup to be performed.
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {
    static const MCFixupKindInfo kBranch{"fixup_mcs6502_branch", 8, 8,
                                         MCFixupKindInfo::FKF_IsPCRel};
    static const MCFixupKindInfo kSymbol8{"fixup_mcs6502_symbol8", 8, 8, 0};
    static const MCFixupKindInfo kSymbol16{"fixup_mcs6502_symbol16", 8, 16, 0};
    static const MCFixupKindInfo kAddrRef{"fixup_mcs6502_addrref", 8, 16, 0};

    if (Kind < FirstTargetFixupKind)
      return MCAsmBackend::getFixupKindInfo(Kind);

    // (Name): name,
    // (TargetOffset): offset into instruction in bits
    // (TargetSize): number of bits that need fixup
    // (Flags): MCFixupKindInfo::FixupKindFlags, or 0 if none
    switch (static_cast<MCS6502::Fixups>(Kind)) {
    case MCS6502::fixup_mcs6502_branch:
      return kBranch;
    case MCS6502::fixup_mcs6502_symbol8:
      return kSymbol8;
    case MCS6502::fixup_mcs6502_symbol16:
      return kSymbol16;
    case MCS6502::fixup_mcs6502_addrref:
      return kAddrRef;
    default:
      assert(false && "Invalid fixup kind!");
    }
  }

  // Relaxation means taking an instruction encoded with some number of bytes
  // and changing the instruction to one that is encoded with more bytes in
  // order to handle its operands. This is perfect for expanding zero page
  // instructions to absolute instructions.
  //
  // If an instruction might need relaxation, it is output to a relaxable
  // fragment, otherwise it's output to a nonrelaxable fragment.
  //
  // All ABS instructions except unconditional jumps (JMP/JSR) can change to
  // ZP instructions. All ABSX instructions can change to ZPX instructions.
  // However, the only ABSY instruction that can change to ZPY is LDX.
  //
  // This is called from MCObjectStreamer::EmitInstructionImpl.
  bool mayNeedRelaxation(const MCInst &Inst,
                         const MCSubtargetInfo &STI) const override {
    bool canBeRelaxed = getRelaxedOpcode(Inst.getOpcode()) != Inst.getOpcode();
    LLVM_DEBUG(dbgs() << "Check needs relaxation:" << canBeRelaxed << "\n");
    return canBeRelaxed;
  }

  void relaxInstruction(const MCInst &Inst, const MCSubtargetInfo &STI,
                        MCInst &Res) const override;

  bool writeNopData(raw_ostream &OS, uint64_t Count) const override;
};

unsigned MCS6502AsmBackend::getRelaxedOpcode(unsigned Opcode) const {
  /* clang-format off */
  switch (Opcode) {
  default: return Opcode;
  case MCS6502::ADC_ABS: return MCS6502::ADC_ZP;
  case MCS6502::STA_ABS: return MCS6502::STA_ZP;
  case MCS6502::LDA_ABS: return MCS6502::LDA_ZP;
  case MCS6502::LDA_ABSX: return MCS6502::LDA_ZPX;
  }
  /* clang-format on */
}

bool MCS6502AsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup,
                                             uint64_t Value,
                                             const MCRelaxableFragment *DF,
                                             const MCAsmLayout &Layout) const {
  LLVM_DEBUG(dbgs() << "fixupNeedsRelaxation? " << Fixup.getKind() << " Value "
                    << Value << "\n");
  if (static_cast<MCS6502::Fixups>(Fixup.getKind()) ==
          MCS6502::fixup_mcs6502_addrref &&
      Value <= 0xFF) {
    LLVM_DEBUG(dbgs() << "fixupNeedsRelaxation!\n");
    return true;
  }
  return false;
}

void MCS6502AsmBackend::relaxInstruction(const MCInst &Inst,
                                         const MCSubtargetInfo &STI,
                                         MCInst &Res) const {
  LLVM_DEBUG(dbgs() << "Relaxing instruction ");
  Inst.print(dbgs());
  Res.setOpcode(getRelaxedOpcode(Inst.getOpcode()));
  Res.addOperand(Inst.getOperand(0));
  LLVM_DEBUG(dbgs() << "\n  Relaxed to ");
  Res.print(dbgs());
  LLVM_DEBUG(dbgs() << "\n");
}

bool MCS6502AsmBackend::writeNopData(raw_ostream &OS, uint64_t Count) const {
  for (int64_t i = 0; i < static_cast<int64_t>(Count); i += 4) {
    OS.write(0xEA);
  }
  return true;
}

// Called from MCAssembler::layout.
void MCS6502AsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                   const MCValue &Target,
                                   MutableArrayRef<char> Data, uint64_t Value,
                                   bool IsResolved,
                                   const MCSubtargetInfo *STI) const {
  LLVM_DEBUG(dbgs() << "Applying fixup " << Fixup.getKind() << " Value "
                    << Value << "\n");

  MCContext &Ctx = Asm.getContext();
  MCFixupKindInfo Info = getFixupKindInfo(Fixup.getKind());

  if (!Value)
    return; // Doesn't change encoding

  if (static_cast<unsigned>(Fixup.getKind()) == MCS6502::fixup_mcs6502_branch) {
    int64_t SignedValue = static_cast<int64_t>(Value);
    if (SignedValue > 0x7F || SignedValue < -0x80) {
      Ctx.reportError(Fixup.getLoc(), "Fixup value for branch out of range");
      return;
    }
  }
  Value <<= Info.TargetOffset;

  unsigned Size = (Info.TargetOffset + Info.TargetSize) / 8;
  unsigned Offset = Fixup.getOffset();

  // For each byte of the machine code bytes that the fixup touches, mask in the
  // bits from the fixup value.
  for (unsigned i = 0; i < Size; i++) {
    Data[Offset + i] |= uint8_t((Value >> (i * 8)) & 0xFF);
  }
}

} // namespace

MCAsmBackend *llvm::createMCS6502AsmBackend(const Target &T,
                                            const MCSubtargetInfo &STI,
                                            const MCRegisterInfo &MRI,
                                            const MCTargetOptions &Options) {
  const Triple &TT = STI.getTargetTriple();
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
  return new MCS6502AsmBackend(OSABI);
}

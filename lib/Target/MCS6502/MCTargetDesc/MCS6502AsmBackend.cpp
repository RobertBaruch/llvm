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

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    return false;
  }

  unsigned getNumFixupKinds() const override {
    return MCS6502::NumTargetFixupKinds;
  }

  // Called from MCAsmStreamer::AddEncodingComment just after encodeInstruction.
  // In the encoding comment, the bytes to be fixed up are symbolized as
  // a letter (e.g. 'A'), and the letter indicates the fixup to be performed.
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {
    static const MCFixupKindInfo kBranch{"fixup_mcs6502_branch", 8, 8,
                                         MCFixupKindInfo::FKF_IsPCRel};
    static const MCFixupKindInfo kSymbol8{"fixup_mcs6502_symbol8", 8, 8, 0};
    static const MCFixupKindInfo kSymbol16{"fixup_mcs6502_symbol16", 8, 16, 0};

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
    default:
      assert(false && "Invalid fixup kind!");
    }
  }

  bool mayNeedRelaxation(const MCInst &Inst,
                         const MCSubtargetInfo &STI) const override {
    return false;
  }

  void relaxInstruction(const MCInst &Inst, const MCSubtargetInfo &STI,
                        MCInst &Res) const override {
    report_fatal_error("MCS6502AsmBackend::relaxInstruction() unimplemented");
  }

  bool writeNopData(raw_ostream &OS, uint64_t Count) const override;
};

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

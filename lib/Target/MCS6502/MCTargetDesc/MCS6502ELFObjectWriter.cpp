//===-- MCS6502ELFObjectWriter.cpp - MCS6502 ELF Writer -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MCS6502FixupKinds.h"
#include "MCTargetDesc/MCS6502MCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "obj-writer"

namespace {

class MCS6502ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  MCS6502ELFObjectWriter(uint8_t OSABI);
  ~MCS6502ELFObjectWriter() override;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};

MCS6502ELFObjectWriter::MCS6502ELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(false /* Is64Bit */, OSABI, ELF::EM_MCS6502,
                              true /* HasRelocationAddend */) {}

MCS6502ELFObjectWriter::~MCS6502ELFObjectWriter() {}

// Returns the relocation type, which are defined in MCS6502.def.
unsigned MCS6502ELFObjectWriter::getRelocType(MCContext &Ctx,
                                              const MCValue &Target,
                                              const MCFixup &Fixup,
                                              bool IsPCRel) const {
  LLVM_DEBUG(dbgs() << "getRelocType for fixup kind " << Fixup.getKind()
                    << "\n");

  // Determine the type of relocation
  switch ((unsigned)Fixup.getKind()) {
  default:
    llvm_unreachable("invalid fixup kind!");
  case MCS6502::fixup_mcs6502_symbol8:
    return ELF::R_MCS6502_SYMBOL8;
  case MCS6502::fixup_mcs6502_symbol16:
    return ELF::R_MCS6502_SYMBOL16;
  case MCS6502::fixup_mcs6502_branch:
    return ELF::R_MCS6502_BRANCH;
  }
}

} // namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createMCS6502ELFObjectWriter(uint8_t OSABI, bool Is64Bit) {
  if (Is64Bit)
    report_fatal_error("64-bit ELF not supported for MCS6502");
  return llvm::make_unique<MCS6502ELFObjectWriter>(OSABI);
}

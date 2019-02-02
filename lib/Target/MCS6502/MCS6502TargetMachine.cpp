//===-- MCS6502TargetMachine.cpp - Define TargetMachine for MCS6502 -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about the MCS6502 target spec.
//
//===----------------------------------------------------------------------===//

#include "MCS6502TargetMachine.h"
#include "MCS6502.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

extern "C" void LLVMInitializeMCS6502Target() {
  RegisterTargetMachine<MCS6502TargetMachine> X(getTheMCS6502Target());
}

static std::string computeDataLayout(const Triple &TT) {
  assert(TT.isArch16Bit() && "only 16 bit is current supported");
  // e: Little-endian
  // m:e: ELF name mangling
  // p:16:16: Pointers are 16 bit, and ABI alignment for pointers is 16 bits.
  // i64:8: 64-bit ints (and lower) are 8-bit aligned (as is the ABI).
  // n8: Native integer width is 8 bits.
  // S8: Stack alignment is 8 bits.
  return "e-m:e-p:16:16-i64:8-n8-S8";
}

static Reloc::Model getEffectiveRelocModel(const Triple &TT,
                                           Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

MCS6502TargetMachine::MCS6502TargetMachine(const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           Optional<Reloc::Model> RM,
                                           Optional<CodeModel::Model> CM,
                                           CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT), TT, CPU, FS, Options,
                        getEffectiveRelocModel(TT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

namespace {

class MCS6502PassConfig : public TargetPassConfig {
public:
  MCS6502PassConfig(MCS6502TargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  MCS6502TargetMachine &getMCS6502TargetMachine() const {
    return getTM<MCS6502TargetMachine>();
  }

  bool addInstSelector() override;
};

} // namespace

bool MCS6502PassConfig::addInstSelector() {
  addPass(createMCS6502ISelDag(getMCS6502TargetMachine()));
  return false;
}

TargetPassConfig *MCS6502TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new MCS6502PassConfig(*this, PM);
}

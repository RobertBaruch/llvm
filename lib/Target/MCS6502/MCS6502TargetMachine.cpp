//===-- MCS6502TargetMachine.cpp - LLVMTargetMachine for MCS6502 ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements MCS6502TargetMachine.
//
//===----------------------------------------------------------------------===//

#include "MCS6502TargetMachine.h"
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
  assert(TT.isArch16Bit() && "only 16 bit is currently supported");
  // e: Little-endian
  // m:e: ELF name mangling
  // p:16:16: Pointers are 16 bits, and ABI alignment for pointers is 16 bits.
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
      TLOF(make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

TargetPassConfig *MCS6502TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new TargetPassConfig(*this, PM);
}

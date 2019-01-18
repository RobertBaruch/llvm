//===-- MCS6502MCTargetDesc.cpp - MCS6502 Target Description --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides MCS6502-specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "MCS6502MCTargetDesc.h"
#include "MCS6502MCAsmInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "MCS6502GenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "MCS6502GenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "MCS6502GenSubtargetInfo.inc"

using namespace llvm;

static MCInstrInfo *createMCS6502MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitMCS6502MCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createMCS6502MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitMCS6502MCRegisterInfo(X, MCS6502::A); // why?
  return X;
}

static MCAsmInfo *createMCS6502MCAsmInfo(const MCRegisterInfo &MRI,
                                         const Triple &TT) {
  return new MCS6502MCAsmInfo(TT);
}

static MCSubtargetInfo *
createMCS6502MCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string CPUName = CPU;
  if (CPUName.empty())
    CPUName = "mcs6502";
  return createMCS6502MCSubtargetInfoImpl(TT, CPUName, FS);
}

extern "C" void LLVMInitializeMCS6502TargetMC() {
  Target *T = &getTheMCS6502Target();

  TargetRegistry::RegisterMCAsmInfo(*T, createMCS6502MCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(*T, createMCS6502MCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(*T, createMCS6502MCRegisterInfo);
  TargetRegistry::RegisterMCAsmBackend(*T, createMCS6502AsmBackend);
  TargetRegistry::RegisterMCCodeEmitter(*T, createMCS6502MCCodeEmitter);
  TargetRegistry::RegisterMCSubtargetInfo(*T, createMCS6502MCSubtargetInfo);
}

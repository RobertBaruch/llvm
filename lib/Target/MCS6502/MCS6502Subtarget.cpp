//===-- MCS6502Subtarget.cpp - MCS6502 subtarget information ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the MCS6502 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "MCS6502Subtarget.h"
#include "MCS6502.h"
#include "MCS6502FrameLowering.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "mcs6502-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "MCS6502GenSubtargetInfo.inc"

void MCS6502Subtarget::anchor() {}

MCS6502Subtarget &
MCS6502Subtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS) {
  // Determine default and user-specific characteristics
  std::string CPUName = CPU;
  if (CPUName.empty())
    CPUName = "generic";
  ParseSubtargetFeatures(CPUName, FS);
  return *this;
}

MCS6502Subtarget::MCS6502Subtarget(const Triple &TT, const std::string &CPU,
                                   const std::string &FS,
                                   const TargetMachine &TM)
    : MCS6502GenSubtargetInfo(TT, CPU, FS),
      FrameLowering(initializeSubtargetDependencies(CPU, FS)), InstrInfo(),
      RegInfo(getHwMode()), TLInfo(TM, *this) {}

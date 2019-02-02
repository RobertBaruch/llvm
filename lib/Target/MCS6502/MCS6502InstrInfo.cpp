//===-- MCS6502InstrInfo.cpp - MCS6502 instruction information --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the MCS6502 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "MCS6502InstrInfo.h"
#include "MCS6502.h"
#include "MCS6502Subtarget.h"
#include "MCS6502TargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "MCS6502GenInstrInfo.inc"

using namespace llvm;

MCS6502InstrInfo::MCS6502InstrInfo() : MCS6502GenInstrInfo() {}

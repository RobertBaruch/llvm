//===-- MCS6502FrameLowering.cpp - MCS6502 frame info ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the MCS6502 implementation of TargetFrameLowering.
//
//===----------------------------------------------------------------------===//

#include "MCS6502FrameLowering.h"
#include "MCS6502Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "frame-lowering"

bool MCS6502FrameLowering::hasFP(const MachineFunction &MF) const {
  return true;
}

void MCS6502FrameLowering::emitPrologue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {}

void MCS6502FrameLowering::emitEpilogue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {}

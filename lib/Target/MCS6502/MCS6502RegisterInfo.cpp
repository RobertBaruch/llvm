//===-- MCS6502RegisterInfo.cpp - MCS6502 register information --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the MCS6502 implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#include "MCS6502RegisterInfo.h"
#include "MCS6502.h"
#include "MCS6502Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "MCS6502GenRegisterInfo.inc"

using namespace llvm;

MCS6502RegisterInfo::MCS6502RegisterInfo(unsigned HwMode)
    : MCS6502GenRegisterInfo(MCS6502::A, 0 /* DwarfFlavor */, 0 /* EHFlavor */,
                             0 /* PC */, HwMode) {}

const MCPhysReg *
MCS6502RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

BitVector
MCS6502RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  // Use markSuperRegs to ensure any register aliases are also reserved

  return Reserved;
}

void MCS6502RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  report_fatal_error("Subroutines not supported yet");
}

unsigned
MCS6502RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  report_fatal_error("No frame register");
}

//=== MCS6502FrameLowering.h - Define frame lowering for MCS6502 -*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class implements the MCS6502-bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MCS6502_MCS6502FRAMELOWERING_H
#define LLVM_LIB_TARGET_MCS6502_MCS6502FRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class MCS6502Subtarget;

class MCS6502FrameLowering : public TargetFrameLowering {
public:
  explicit MCS6502FrameLowering(const MCS6502Subtarget &STI)
      : TargetFrameLowering(StackGrowsDown, 8 /* StackAlignment */,
                            0 /* LocalAreaOffset */) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MCS6502_MCS6502FRAMELOWERING_H

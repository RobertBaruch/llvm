//===-- MCS6502.h - Top-level interface for MCS6502 -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// MCS6502 back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MCS6502_MCS6502_H
#define LLVM_LIB_TARGET_MCS6502_MCS6502_H

#include "MCTargetDesc/MCS6502MCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class MCS6502TargetMachine;
class MCInst;
class MachineInstr;

void LowerMCS6502MachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI);

FunctionPass *createMCS6502ISelDag(MCS6502TargetMachine &TM);

} // namespace llvm

#endif // LLVM_LIB_TARGET_MCS6502_MCS6502_H

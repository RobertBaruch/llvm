//=== MCS6502InstrInfo.h - MCS6502 instruction information -------*- C++ -*-==//
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

#ifndef LLVM_LIB_TARGET_MCS6502_MCS6502INSTRINFO_H
#define LLVM_LIB_TARGET_MCS6502_MCS6502INSTRINFO_H

#include "MCS6502RegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "MCS6502GenInstrInfo.inc"

namespace llvm {

class MCS6502InstrInfo : public MCS6502GenInstrInfo {
public:
  MCS6502InstrInfo();
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MCS6502_MCS6502INSTRINFO_H

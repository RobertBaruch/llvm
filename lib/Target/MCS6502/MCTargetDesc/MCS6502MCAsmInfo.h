//===-- MCS6502MCAsmInfo.h - MCS6502 Asm Info -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the MCS6502MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502ASMINFO_H
#define LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502ASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class MCS6502MCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit MCS6502MCAsmInfo(const Triple &TargetTriple);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502ASMINFO_H

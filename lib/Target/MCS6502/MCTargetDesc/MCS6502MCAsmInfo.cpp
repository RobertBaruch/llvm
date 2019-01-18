//===-- MCS6502MCAsmInfo.cpp - MCS6502 Asm Properties ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the definitions of the MCS6502MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#include "MCS6502MCAsmInfo.h"
#include "llvm/ADT/Triple.h"

using namespace llvm;

void MCS6502MCAsmInfo::anchor() {}

MCS6502MCAsmInfo::MCS6502MCAsmInfo(const Triple &TT) {
  CodePointerSize = 2;
  CommentString = ";";
  AlignmentIsInBytes = true;
  SupportsDebugInformation = true;
}

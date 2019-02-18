//===-- MCS6502FixupKinds.h - MCS6502 Fixup Entries -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides MCS6502 specific fixup enumerations.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502FIXUPKINDS_H
#define LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502FIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

// I guess in case this is defined, it doesn't interfere with the namespace?
#undef MCS6502

namespace llvm {
namespace MCS6502 {

enum Fixups {
  // Symbol references in branch instructions
  fixup_mcs6502_branch = FirstTargetFixupKind,
  fixup_mcs6502_abs_label, // Symbol references for JMP and JSR
  fixup_mcs6502_symbol8,   // A symbol that must be 8 bits
  fixup_mcs6502_symbol16,  // A symbol that must be 16 bits
  fixup_mcs6502_invalid,   // Sentinel value for last fixup
  NumTargetFixupKinds = fixup_mcs6502_invalid - FirstTargetFixupKind
};

} // namespace MCS6502
} // namespace llvm

#endif // LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502FIXUPKINDS_H
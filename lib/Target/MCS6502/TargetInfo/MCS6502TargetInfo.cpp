//===-- MCS6502TargetInfo.cpp - MCS6502 Target implementation -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace llvm {

Target &getTheMCS6502Target() {
  static Target TheMCS6502Target;
  return TheMCS6502Target;
}

extern "C" void LLVMInitializeMCS6502TargetInfo() {
  RegisterTarget<Triple::mcs6502> X(getTheMCS6502Target(), "mcs6502",
                                    "MOS MCS6502", "MCS6502");
}

} // namespace llvm
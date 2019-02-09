//===-- MCS6502TargetInfo.cpp - MCS6502 target information ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
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

/// FIXME: Temporary stub. Required for linking to succeed. Remove once
/// properly implemented.
extern "C" void LLVMInitializeMCS6502TargetMC() {}

} // namespace llvm

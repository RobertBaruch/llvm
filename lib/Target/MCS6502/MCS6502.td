//===-- MCS6502.td - The MCS6502 Target Machine ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Registers, instruction descriptions.
//===----------------------------------------------------------------------===//

include "MCS6502RegisterInfo.td"
include "MCS6502InstrInfo.td"

def MCS6502InstrInfo : InstrInfo;

def MCS6502AsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 0;
}

//===----------------------------------------------------------------------===//
// MCS6502 processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", NoSchedModel, []>;

//===----------------------------------------------------------------------===//
// Define the MCS602 target.
//===----------------------------------------------------------------------===//

def MCS6502 : Target {
  let InstructionSet = MCS6502InstrInfo;
  let AssemblyParsers = [MCS6502AsmParser];
}

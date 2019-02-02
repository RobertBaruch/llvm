//===-- MCS6502.td - Describe the MCS6502 Target Machine ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// MCS6502 subtarget features and instruction predicates.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Register file, instruction descriptions.
//===----------------------------------------------------------------------===//

include "MCS6502RegisterInfo.td"
include "MCS6502CallingConv.td"
include "MCS6502InstrInfo.td"

//===----------------------------------------------------------------------===//
// MCS6502 processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", NoSchedModel, []>;

//===----------------------------------------------------------------------===//
// Define the MCS6502 target.
//===----------------------------------------------------------------------===//

def MCS6502InstrInfo : InstrInfo {
  let guessInstructionProperties = 0;
}

def MCS6502AsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 0;
}

def MCS6502 : Target {
  let InstructionSet = MCS6502InstrInfo;
  let AssemblyParsers = [MCS6502AsmParser];
}

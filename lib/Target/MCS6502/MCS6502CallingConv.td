//=== MCS6502CallingConv.td - MCS6502 Callong Convention ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the calling conventions for the MCS6502 architecture.
//
//===----------------------------------------------------------------------===//

// MCS6502 C return-value convention.
def RetCC_MCS6502 : CallingConv<[
  CCIfType<[i8], CCAssignToReg<[A, X]>>
  //,
  //CCIfType<[i16], CCAssignToReg<[A, X]>>
]>;

// MCS6502 C calling convention.
def CC_MCS6502 : CallingConv<[
  CCIfType<[i8], CCAssignToReg<[A, X]>>
]>;

// Callee-saved registers
def CSR : CalleeSavedRegs<(add Y)>;

//===-- MCS6502RegisterInfo.td - MCS6502 Register defs -----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Declarations that describe the MCS6502 registers
//===----------------------------------------------------------------------===//

let Namespace="MCS6502" in {

class MCS6502Reg<string n> : Register<n> {
}

def A : MCS6502Reg<"A">;
def X : MCS6502Reg<"X">;
def Y : MCS6502Reg<"Y">;

} // Namespace "MCS6502"

def GPR : RegisterClass<"MCS6502", [i8], 8, (add A, X, Y)>;

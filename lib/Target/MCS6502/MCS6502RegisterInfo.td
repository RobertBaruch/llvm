//===-- MCS6502RegisterInfo.td - MCS6502 Registers ---------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

let Namespace="MCS6502" in {

class MCS6502Reg<string n> : Register<n> {
}

class MCS6502PhysicalReg<string n>: MCS6502Reg<n> {
}

class MCS6502PseudoReg<string n>: MCS6502Reg<n> {
}

def A : MCS6502PhysicalReg<"A">;
def X : MCS6502PhysicalReg<"X">;
def Y : MCS6502PhysicalReg<"Y">;

foreach i = 0-31 in {
  def X#i : MCS6502PseudoReg<"X"#i>;
}

} // namespace MCS6502

def PseudoReg : RegisterClass<"MCS6502", [i8], 8, 
  (add (sequence "X%u", 0, 31))>;

def PhysicalReg : RegisterClass<"MCS6502", [i8], 8, (add A, X, Y)>;

def AccReg : RegisterClass<"MCS6502", [i8], 8, (add A)>;
def ZPXReg : RegisterClass<"MCS6502", [i8], 8, (add X)>;
def ZPYReg : RegisterClass<"MCS6502", [i8], 8, (add Y)>;
def AbsXReg : RegisterClass<"MCS6502", [i8], 8, (add X)>;
def AbsYReg : RegisterClass<"MCS6502", [i8], 8, (add Y)>;
def PreIndReg : RegisterClass<"MCS6502", [i8], 8, (add X)>;
def PostIndReg : RegisterClass<"MCS6502", [i8], 8, (add Y)>;

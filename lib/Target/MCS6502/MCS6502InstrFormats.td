//=== MCS6502InstrFormats.td - MCS6502 Instruction Formats -*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

/// A mode enumeration.
class MCS6502Mode<int n, string description, string argstr, dag ins> {
  int Value = n;
  string Description = description;
  string Argstr = argstr;
  dag InOperandList = ins;
}

def MODE_IMP  : MCS6502Mode<0, "Implied", "", (ins)>;
def MODE_IMM  : MCS6502Mode<1, "Immediate", "#$$$imm8", (ins i8imm:$imm8)>;
def MODE_ZP   : MCS6502Mode<2, "Zero Page", "$$$addr8", (ins i8imm:$addr8)>;
def MODE_ZPX  : MCS6502Mode<3, "Zero Page, X", "$$$addr8,X", (ins i8imm:$addr8)>;
def MODE_ZPY  : MCS6502Mode<3, "Zero Page, Y", "$$$addr8,Y", (ins i8imm:$addr8)>;
def MODE_ABS  : MCS6502Mode<4, "Absolute", "$$$addr16", (ins i16imm:$addr16)>;
def MODE_ABSX : MCS6502Mode<5, "Absolute, X", "$$$addr16,X", (ins i16imm:$addr16)>;
def MODE_ABSY : MCS6502Mode<6, "Absolute, Y", "$$$addr16,Y", (ins i16imm:$addr16)>;
def MODE_INDX : MCS6502Mode<7, "Indirect, X", "($$$addr16,X)", (ins i16imm:$addr16)>;
def MODE_INDY : MCS6502Mode<8, "Indirect, Y", "( $addr ),Y", (ins i16imm:$addr)>;
def MODE_ACC  : MCS6502Mode<9, "Accumulator", "", (ins)>;
def MODE_REL  : MCS6502Mode<10, "Relative", "#$$$simm8", (ins i8imm:$simm8)>;
def MODE_IND  : MCS6502Mode<11, "Indirect", "( $addr )", (ins i16imm:$addr)>;

/// A holder for opcode value, string, and mode.
class MCS6502Opcode<bits<8> val, string opcodestr, MCS6502Mode mode> {
  bits<8> Value = val;
  string OpcodeStr = opcodestr;
  MCS6502Mode Mode = mode;
}

/// A general instruction.
class MCS6502Inst<dag outs, MCS6502Opcode opcode,
                  list<dag> pattern = []> : Instruction {
  field bits<24> Inst; // max size 3 bytes
  MCS6502Opcode Opcode = opcode;
  dag OutOperandList = outs;
  dag InOperandList = opcode.Mode.InOperandList;

  let Namespace = "MCS6502";
  let AsmString = opcode.OpcodeStr # " " # opcode.Mode.Argstr;
  let Pattern = pattern;
  let hasSideEffects = 0;
  let mayStore = 0;
  let mayLoad = 0;
}

/// A 1-byte instruction.
class MCS6502Inst1Byte<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  let Inst{7-0} = Opcode.Value;
  let Size = 1;
}

/// A 2-byte instruction.
class MCS6502Inst2Byte<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  bits<8> Arg;

  let Inst{15-8} = Opcode.Value;
  let Inst{7-0} = Arg;
  let Size = 2;
}

/// A 3-byte instruction.
class MCS6502Inst3Byte<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  bits<16> Arg;
  
  let Inst{23-16} = Opcode.Value;
  let Inst{15-0} = Arg;
  let Size = 3;
}

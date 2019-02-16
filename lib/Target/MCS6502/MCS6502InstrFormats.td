//===-- MCS6502InstrFormats.td - MCS6502 Instr Formats -----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// AsmOperand types
//===----------------------------------------------------------------------===//

def RelAsmOperand : AsmOperandClass {
  let Name = "Rel";
}

def ZPAsmOperand : AsmOperandClass {
  let Name = "ZP";
}

def AbsAsmOperand : AsmOperandClass {
  let Name = "Abs";
}

def PreIndAsmOperand : AsmOperandClass {
  let Name = "PreInd";
}

def PostIndAsmOperand : AsmOperandClass {
  let Name = "PostInd";
}

def IndAsmOperand : AsmOperandClass {
  let Name = "Ind";
}

def AbsXAsmOperand : AsmOperandClass {
  let Name = "AbsX";
}

def AbsYAsmOperand : AsmOperandClass {
  let Name = "AbsY";
}

def ZPXAsmOperand : AsmOperandClass {
  let Name = "ZPX";
}

def ZPYAsmOperand : AsmOperandClass {
  let Name = "ZPY";
}

//===----------------------------------------------------------------------===//
// Operand types
//===----------------------------------------------------------------------===//

def immediate : Operand<i8> {
  let PrintMethod = "printImmediate";
}

def rel : Operand<i8> {
  let PrintMethod = "printImmediate";
  let ParserMatchClass = RelAsmOperand;
}

def zp : Operand<i8> {
  let PrintMethod = "printImmediate";
  let ParserMatchClass = ZPAsmOperand;
}

def absolute : Operand<i16> {
  let PrintMethod = "printImmediate16";
  let ParserMatchClass = AbsAsmOperand;
}

def ind : Operand<i16> {
  let PrintMethod = "printImmediate16";
  let ParserMatchClass = IndAsmOperand;
}

def zp_preind : Operand<i8> {
  let ParserMatchClass = PreIndAsmOperand;
}

def zp_postind : Operand<i8> {
  let ParserMatchClass = PostIndAsmOperand;
}

def absx : Operand<i16> {
  let ParserMatchClass = AbsXAsmOperand;
}

def absy : Operand<i16> {
  let ParserMatchClass = AbsYAsmOperand;
}

def zpx : Operand<i8> {
  let ParserMatchClass = ZPXAsmOperand;
}

def zpy : Operand<i8> {
  let ParserMatchClass = ZPYAsmOperand;
}

//===----------------------------------------------------------------------===//
// Modes
//===----------------------------------------------------------------------===//

class Mode<int n, string description, string argstr, dag ins> {
  int Value = n;
  string Description = description;
  string Argstr = argstr;
  dag InOperandList = ins;
}

def MODE_IMP  : Mode<0, "Implied", "", (ins)>;
def MODE_IMM  : Mode<1, "Immediate", "#$val8", (ins immediate:$val8)>;
def MODE_ZP   : Mode<2, "Zero Page", "$val8", (ins zp:$val8)>;
def MODE_ZPX  : Mode<3, "Zero Page, X", 
    "$val8, $reg", (ins zpx:$val8, ZPXReg:$reg)>;
def MODE_ZPY  : Mode<4, "Zero Page, Y", 
    "$val8, $reg", (ins zpy:$val8, ZPYReg:$reg)>;
def MODE_ABS  : Mode<5, "Absolute", "$val16", (ins absolute:$val16)>;
def MODE_ABSX :
    Mode<6, "Absolute, X", "$val16, $reg", (ins absx:$val16, AbsXReg:$reg)>;
def MODE_ABSY : 
    Mode<7, "Absolute, Y", "$val16, $reg", (ins absy:$val16, AbsYReg:$reg)>;
def MODE_POSTIND : Mode<8, "Indirect Zero Page, Y",
    "(${val8}), $reg", (ins zp_postind:$val8, PostIndReg:$reg)>;
def MODE_PREIND : Mode<9, "Indirect Zero Page, X",
    "($val8, ${reg})", (ins zp_preind:$val8, PreIndReg:$reg)>;
def MODE_ACC  : Mode<10, "Accumulator", "$reg", (ins AccReg:$reg)>;
def MODE_REL  : Mode<11, "Relative", "$val8", (ins rel:$val8)>;
def MODE_IND : Mode<12, "Indirect", "(${val16})", (ins ind:$val16)>;

//===----------------------------------------------------------------------===//
// Instruction types
//===----------------------------------------------------------------------===//

class Inst1Byte<string mnemonic, bits<8> opcode, Mode mode,
    dag outs, list<dag> patterns = []> : Instruction {
  let Namespace = "MCS6502";
  let Size = 1;

  bits<8> Inst;

  let Inst{7-0} = opcode;
  let AsmString = mnemonic # " " # mode.Argstr;
  let OutOperandList = outs;
  let InOperandList = mode.InOperandList;
}

class Inst2Byte<string mnemonic, bits<8> opcode, Mode mode,
    dag outs, list<dag> patterns = []> : Instruction {
  let Namespace = "MCS6502";
  let Size = 2;

  bits<16> Inst;
  bits<8> val8;

  let Inst{15-8} = opcode;
  let Inst{7-0} = val8;
  let AsmString = mnemonic # " " # mode.Argstr;
  let OutOperandList = outs;
  let InOperandList = mode.InOperandList;
}

class Inst3Byte<string mnemonic, bits<8> opcode, Mode mode,
    dag outs, list<dag> patterns = []> : Instruction {
  let Namespace = "MCS6502";
  let Size = 3;

  bits<24> Inst;
  bits<16> val16;

  let Inst{23-16} = opcode;
  let Inst{15-8} = val16{7-0};
  let Inst{7-0} = val16{15-8};
  let AsmString = mnemonic # " " # mode.Argstr;
  let OutOperandList = outs;
  let InOperandList = mode.InOperandList;
}

class imp_inst<string mnemonic, bits<8> opcode> : Inst1Byte<mnemonic, opcode,
    MODE_IMP, (outs)>;
    
// See also: nparker.llx.com/a2/opcodes.html

multiclass alu_inst<string mnemonic, bits<3> aaa> {
  def _INDX : Inst2Byte<mnemonic, {aaa, 0b000, 0b01}, MODE_PREIND, (outs)>;
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b01}, MODE_ZP, (outs)>;
  def _IMM  : Inst2Byte<mnemonic, {aaa, 0b010, 0b01}, MODE_IMM, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b01}, MODE_ABS, (outs)>;
  def _INDY : Inst2Byte<mnemonic, {aaa, 0b100, 0b01}, MODE_POSTIND, (outs)>;
  def _ZPX  : Inst2Byte<mnemonic, {aaa, 0b101, 0b01}, MODE_ZPX, (outs)>;
  def _ABSY : Inst3Byte<mnemonic, {aaa, 0b110, 0b01}, MODE_ABSY, (outs)>;
  def _ABSX : Inst3Byte<mnemonic, {aaa, 0b111, 0b01}, MODE_ABSX, (outs)>;
}

multiclass sta_inst<string mnemonic, bits<3> aaa> {
  def _INDX : Inst2Byte<mnemonic, {aaa, 0b000, 0b01}, MODE_PREIND, (outs)>;
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b01}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b01}, MODE_ABS, (outs)>;
  def _INDY : Inst2Byte<mnemonic, {aaa, 0b100, 0b01}, MODE_POSTIND, (outs)>;
  def _ZPX  : Inst2Byte<mnemonic, {aaa, 0b101, 0b01}, MODE_ZPX, (outs)>;
  def _ABSY : Inst3Byte<mnemonic, {aaa, 0b110, 0b01}, MODE_ABSY, (outs)>;
  def _ABSX : Inst3Byte<mnemonic, {aaa, 0b111, 0b01}, MODE_ABSX, (outs)>;
}

multiclass shift_inst<string mnemonic, bits<3> aaa> {
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b10}, MODE_ZP, (outs)>;
  def _ACC  : Inst1Byte<mnemonic, {aaa, 0b010, 0b10}, MODE_ACC, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b10}, MODE_ABS, (outs)>;
  def _ZPX  : Inst2Byte<mnemonic, {aaa, 0b101, 0b10}, MODE_ZPX, (outs)>;
  def _ABSX : Inst3Byte<mnemonic, {aaa, 0b111, 0b10}, MODE_ABSX, (outs)>;
}

multiclass inc_dec_inst<string mnemonic, bits<3> aaa> {
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b10}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b10}, MODE_ABS, (outs)>;
  def _ZPX  : Inst2Byte<mnemonic, {aaa, 0b101, 0b10}, MODE_ZPX, (outs)>;
  def _ABSX : Inst3Byte<mnemonic, {aaa, 0b111, 0b10}, MODE_ABSX, (outs)>;
}

multiclass ldx_inst<string mnemonic, bits<3> aaa> {
  def _IMM  : Inst2Byte<mnemonic, {aaa, 0b000, 0b10}, MODE_IMM, (outs)>;
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b10}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b10}, MODE_ABS, (outs)>;
  def _ZPY  : Inst2Byte<mnemonic, {aaa, 0b101, 0b10}, MODE_ZPY, (outs)>;
  def _ABSY : Inst3Byte<mnemonic, {aaa, 0b111, 0b10}, MODE_ABSY, (outs)>;
}

multiclass stx_inst<string mnemonic, bits<3> aaa> {
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b10}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b10}, MODE_ABS, (outs)>;
  def _ZPY  : Inst2Byte<mnemonic, {aaa, 0b101, 0b10}, MODE_ZPY, (outs)>;
}

multiclass bit_inst<string mnemonic, bits<3> aaa> {
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b00}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b00}, MODE_ABS, (outs)>;
}

multiclass sty_inst<string mnemonic, bits<3> aaa> {
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b00}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b00}, MODE_ABS, (outs)>;
  def _ZPX  : Inst2Byte<mnemonic, {aaa, 0b101, 0b00}, MODE_IND, (outs)>;
}

multiclass ldy_inst<string mnemonic, bits<3> aaa> {
  def _IMM  : Inst2Byte<mnemonic, {aaa, 0b000, 0b00}, MODE_IMM, (outs)>;
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b00}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b00}, MODE_ABS, (outs)>;
  def _ZPX  : Inst2Byte<mnemonic, {aaa, 0b101, 0b00}, MODE_ZPX, (outs)>;
  def _ABSX : Inst3Byte<mnemonic, {aaa, 0b111, 0b00}, MODE_ABSX, (outs)>;
}

multiclass cpx_cpy_inst<string mnemonic, bits<3> aaa> {
  def _IMM  : Inst2Byte<mnemonic, {aaa, 0b000, 0b00}, MODE_IMM, (outs)>;
  def _ZP   : Inst2Byte<mnemonic, {aaa, 0b001, 0b00}, MODE_ZP, (outs)>;
  def _ABS  : Inst3Byte<mnemonic, {aaa, 0b011, 0b00}, MODE_ABS, (outs)>;
}


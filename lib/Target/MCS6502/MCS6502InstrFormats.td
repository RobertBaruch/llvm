//=== MCS6502InstrFormats.td - MCS6502 Instruction Formats -*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Operands
//===----------------------------------------------------------------------===//

// Each of these AsmOperandClasses requires an is<Name> function in
// MCS6502Operand (MCS6502AsmParser.cpp) by default.
def Imm8AsmOperand : AsmOperandClass {
  let Name = "Imm8";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def SImm8AsmOperand : AsmOperandClass {
  let Name = "SImm8";
  let RenderMethod = "addSImm8Operands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def Imm16AsmOperand : AsmOperandClass {
  let Name = "Imm16";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def Addr16AsmOperand : AsmOperandClass {
  let Name = "Addr16";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

//===----------------------------------------------------------------------===//
// Operand names
//===----------------------------------------------------------------------===//

// We don't want to make the Operand base class type i8 or i16 because then they
// will all match using the "generic matcher". For example, LDA_ZP and LDA_ABS
// will match in essentially a random way given LDA <imm>, and the matcher
// chooses the first instruction that matches. You need each operand to be
// uniquely determinable.

// Unsigned 8-bit value
def imm8 : Operand<OtherVT> {
  let ParserMatchClass = Imm8AsmOperand;
  let DecoderMethod = "decodeUImmOperand<8>";
}

// Signed 8-bit value
def simm8 : Operand<OtherVT> {
  let ParserMatchClass = SImm8AsmOperand;
  let DecoderMethod = "decodeSImmOperand<8>";
  let PrintMethod = "printSImmOperand";
}

// Unsigned 16-bit value
def imm16 : Operand<OtherVT> {
  let ParserMatchClass = Imm16AsmOperand;
  let DecoderMethod = "decodeUImmOperand<16>";
}

// This can be an 8- or 16-bit address.
def addr16 : Operand<OtherVT> {
  let ParserMatchClass = Addr16AsmOperand;
  let DecoderMethod = "decodeUImmOperand<16>";
}

//===----------------------------------------------------------------------===//
// Modes
//===----------------------------------------------------------------------===//

/// A mode enumeration.
class MCS6502Mode<int n, string description, string argstr, dag ins> {
  int Value = n;
  string Description = description;
  string Argstr = argstr;
  dag InOperandList = ins;
}

// Oh hey, where the gen-asm-matcher happily accepted \\(, the gen-asm-printer
// doesn't! No documentation, no consistency.

// We originally tried having a MODE_ZP_FAKE so that we could match jmp/jsr
// for both 8-bit and 16-bit operands and encode them both with 16-bit operands.
// However, this caused a matcher conflict in the disassembler, because both
// jmp zp and jmp abs decode the same way. So we're going to have to make these
// instructions have a completely custom mode and operand type.

def MODE_IMP  : MCS6502Mode<0, "Implied", "", (ins)>;
def MODE_IMM  : MCS6502Mode<1, "Immediate", "#$imm8", (ins imm8:$imm8)>;
def MODE_ZP   : MCS6502Mode<2, "Zero Page", "$imm8", (ins imm8:$imm8)>;
def MODE_ZPX  : MCS6502Mode<3, "Zero Page, X", "$imm8,X", (ins imm8:$imm8)>;
def MODE_ZPY  : MCS6502Mode<3, "Zero Page, Y", "$imm8,Y", (ins imm8:$imm8)>;
def MODE_ABS  : MCS6502Mode<4, "Absolute", "$addr16", (ins imm16:$addr16)>;
def MODE_ABSX : MCS6502Mode<5, "Absolute, X", "$addr16,X", (ins imm16:$addr16)>;
def MODE_ABSY : MCS6502Mode<6, "Absolute, Y", "$addr16,Y", (ins imm16:$addr16)>;
def MODE_INDX : MCS6502Mode<7, "Indirect, X", "( $addr16,X )", (ins imm16:$addr16)>;
def MODE_INDY : MCS6502Mode<8, "Indirect, Y", "( $addr16 ),Y", (ins imm16:$addr16)>;
def MODE_ACC  : MCS6502Mode<9, "Accumulator", "", (ins)>;
def MODE_REL  : MCS6502Mode<10, "Relative", "#$imm8", (ins simm8:$imm8)>;
def MODE_IND  : MCS6502Mode<11, "Indirect", "( $addr16 )", (ins imm16:$addr16)>;
def MODE_ADDR : MCS6502Mode<11, "Absolute", "$addr16", (ins addr16:$addr16)>;

//===----------------------------------------------------------------------===//
// Instruction formats
//===----------------------------------------------------------------------===//

/// A holder for opcode value, string, and mode.
class MCS6502Opcode<bits<8> val, string opcodestr, MCS6502Mode mode> {
  bits<8> Value = val;
  string OpcodeStr = opcodestr;
  MCS6502Mode Mode = mode;
}

/// A general instruction.
class MCS6502Inst<dag outs, MCS6502Opcode opcode,
                  list<dag> pattern = []> : Instruction {
  MCS6502Opcode Opcode = opcode;
  dag OutOperandList = outs;
  dag InOperandList = opcode.Mode.InOperandList;

  let Namespace = "MCS6502";
  let AsmString = opcode.OpcodeStr # " " # opcode.Mode.Argstr;
  let Pattern = pattern;
  let hasSideEffects = 0;
  let mayStore = 0;
  let mayLoad = 0;
  // SoftFail is a field the disassembler can use to provide a way for
  // instructions to not match without killing the whole decode process. It is
  // mainly used for ARM, but tablegen expects this field to exist or it failos
  // to build the decode table!
  field bits<32> SoftFail = 0;
}

/// A 1-byte instruction.
class MCS6502Inst1Byte<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<8> Inst;
  let Inst{7-0} = Opcode.Value;
  let Size = 1;
}

/// A 2-byte instruction.
class MCS6502Inst2Byte<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<16> Inst;
  bits<8> imm8; // This name has to match the $name in the args.

  let Inst{15-8} = Opcode.Value;
  let Inst{7-0} = imm8;
  let Size = 2;
}

/// A 3-byte instruction.
class MCS6502Inst3Byte<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<24> Inst; // max size 3 bytes
  bits<16> addr16;
  
  let Inst{23-16} = Opcode.Value;
  let Inst{15-8} = addr16{7-0};
  let Inst{7-0} = addr16{15-8};
  let Size = 3;
}

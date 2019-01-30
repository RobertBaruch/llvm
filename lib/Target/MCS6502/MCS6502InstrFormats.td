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
//
// The Name field becomes an "MCK_<name>" MatchClassKind enum in
// MCS6502GenAsmMatcher.inc.
//
// The RenderMethod field is the method in MCS6502Operand that converts the
// MCS6502Operand into an MCOperand and adds it to an MCInst. It is called in
// MCS6502GenAsmMatcher.inc, convertToMCInst(), which is called during
// MatchInstructionImpl(), which is generally called by
// MatchAndEmitInstruction().
//
// The DiagnosticType is a string for "custom match classes", whatever those
// are. With a DiagnosticString, it becomes a 
// <Target>AsmParser::Match_<DiagnosticType> enum.
// Used during AsmMatcherEmitter::emitOperandMatchErrorDiagStrings.
def Imm8AsmOperand : AsmOperandClass {
  let Name = "Imm8";
  let RenderMethod = "addImmOperands";
//  let DiagnosticType = !strconcat("Invalid", Name);
}

def Imm16AsmOperand : AsmOperandClass {
  let Name = "Imm16";
  let RenderMethod = "addImmOperands";
}

def Addr16AsmOperand : AsmOperandClass {
  let Name = "Addr16";
  let RenderMethod = "addImmOperands";
}

// For now, all the operands will be added as an expression via
// addImmOperands().
def ImmediateAsmOperand : AsmOperandClass {
  let Name = "Immediate";
  let RenderMethod = "addImmOperands";
}

def Addr8AsmOperand : AsmOperandClass {
  let Name = "Addr8";
  let RenderMethod = "addImmOperands";
}

def Addr8XAsmOperand : AsmOperandClass {
  let Name = "Addr8X";
  let RenderMethod = "addImmOperands";
}

def Addr8YAsmOperand : AsmOperandClass {
  let Name = "Addr8Y";
  let RenderMethod = "addImmOperands";
}

// These are all 16-bit addresses.
def AddrAsmOperand : AsmOperandClass {
  let Name = "Addr";
  let RenderMethod = "addImmOperands";
}

def AddrXAsmOperand : AsmOperandClass {
  let Name = "AddrX";
  let RenderMethod = "addImmOperands";
}

def AddrYAsmOperand : AsmOperandClass {
  let Name = "AddrY";
  let RenderMethod = "addImmOperands";
}

def IndAddrAsmOperand : AsmOperandClass {
  let Name = "IndAddr";
  let RenderMethod = "addImmOperands";
}

def IndXAddrAsmOperand : AsmOperandClass {
  let Name = "IndXAddr";
  let RenderMethod = "addImmOperands";
}

def IndYAddrAsmOperand : AsmOperandClass {
  let Name = "IndYAddr";
  let RenderMethod = "addImmOperands";
}


//===----------------------------------------------------------------------===//
// Operand names
//===----------------------------------------------------------------------===//

// We don't want to make the Operand base class type i8 or i16 because then they
// will all match using the "generic matcher". For example, LDA_ZP and LDA_ABS
// will match in essentially a random way given LDA <imm>, and the matcher
// chooses the first instruction that matches. You need each operand to be
// uniquely determinable.
//
// The ParserMatchClasses are subclasses of AsmOperandClass, defined above.
//
// The PrintMethod is how the Operand will be printed via
// MCS6502GenAsmWriter.inc and MCS6502InstPrinter. Although you lose information
// about the operands when your instruction is represented as an MCInst and
// your operands as MCOperands, the generated <Target>GenAsmWriter remembers
// which of these Operands go with which opcode. So really the PrintMethod
// dictates how to print an MCInst's MCOperand as an Operand.
//
// The DecoderMethod (if specified) is how the machine code operand is
// decoded. The function goes in to MCS6502Disassembler.

// Unsigned 8-bit value
def imm8 : Operand<OtherVT> {
  let ParserMatchClass = Imm8AsmOperand;
  let DecoderMethod = "decodeUImmOperand<8>";
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

def addr8 : Operand<OtherVT> {
  let ParserMatchClass = Addr8AsmOperand;
  let PrintMethod = "printAddress8";
}

def addr8x : Operand<OtherVT> {
  let ParserMatchClass = Addr8XAsmOperand;
  let PrintMethod = "printAddress8X";
}

def addr8y : Operand<OtherVT> {
  let ParserMatchClass = Addr8YAsmOperand;
  let PrintMethod = "printAddress8Y";
}

def immed : Operand<OtherVT> {
  let ParserMatchClass = ImmediateAsmOperand;
  let PrintMethod = "printImmediate";
}

def addr : Operand<OtherVT> {
  let ParserMatchClass = AddrAsmOperand;
  let PrintMethod = "printAddress";
}

def addrx : Operand<OtherVT> {
  let ParserMatchClass = AddrXAsmOperand;
  let PrintMethod = "printAddressX";
}

def addry : Operand<OtherVT> {
  let ParserMatchClass = AddrYAsmOperand;
  let PrintMethod = "printAddressY";
}

def indaddr : Operand<OtherVT> {
  let ParserMatchClass = IndAddrAsmOperand;
  let PrintMethod = "printIndAddress";
}

def indxaddr : Operand<OtherVT> {
  let ParserMatchClass = IndXAddrAsmOperand;
  let PrintMethod = "printIndXAddress";
}

def indyaddr : Operand<OtherVT> {
  let ParserMatchClass = IndYAddrAsmOperand;
  let PrintMethod = "printIndYAddress";
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

// For instructions that have both zero page and absolute modes, we can't tell
// which should be used unless we can evaluate the operand immediately.
// Otherwise it's going to have to be a fixup (maybe) for later.

// The types in type:$name are subclasses of Operand, defined above.
// The $names in type:$name are used in the Instruction definition below.

def MODE_IMP  : MCS6502Mode<0, "Implied", "", (ins)>;
def MODE_IMM  : MCS6502Mode<1, "Immediate", "$imm8", (ins immed:$imm8)>;
def MODE_ZP   : MCS6502Mode<2, "Zero Page", "$addr", (ins addr8:$addr)>;
def MODE_ZPX  : MCS6502Mode<3, "Zero Page, X", "$addr", (ins addr8x:$addr)>;
def MODE_ZPY  : MCS6502Mode<3, "Zero Page, Y", "$addr", (ins addr8y:$addr)>;
def MODE_ABS  : MCS6502Mode<4, "Absolute", "$addr16", (ins addr:$addr16)>;
def MODE_ABSX : MCS6502Mode<5, "Absolute, X", "$addr16", (ins addrx:$addr16)>;
def MODE_ABSY : MCS6502Mode<6, "Absolute, Y", "$addr16", (ins addry:$addr16)>;
def MODE_INDX : MCS6502Mode<7, "Indirect, X", "$addr", (ins indxaddr:$addr)>;
def MODE_INDY : MCS6502Mode<8, "Indirect, Y", "$addr", (ins indyaddr:$addr)>;
def MODE_ACC  : MCS6502Mode<9, "Accumulator", "", (ins)>;
def MODE_REL  : MCS6502Mode<10, "Relative", "$imm8", (ins addr:$imm8)>;
def MODE_IND  : MCS6502Mode<11, "Indirect", "$addr16", (ins indaddr:$addr16)>;

//===----------------------------------------------------------------------===//
// Instruction formats
//===----------------------------------------------------------------------===//

/// A holder for opcode value, string, and mode.
class MCS6502Opcode<bits<8> val, string opcodestr, MCS6502Mode mode> {
  bits<8> Value = val;
  string OpcodeStr = opcodestr;
  MCS6502Mode Mode = mode;
}

/// A general instruction. Instruction is defined in Target.td.
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

/// A 1-byte return-from-call instruction.
class MCS6502Inst1ByteRet<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<8> Inst;
  let Inst{7-0} = Opcode.Value;
  let Size = 1;
  let isReturn = 1;
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

/// A 2-byte zero-page instruction.
class MCS6502Inst2ByteZP<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<16> Inst;
  bits<8> addr; // This name has to match the $name in the args.

  let Inst{15-8} = Opcode.Value;
  let Inst{7-0} = addr;
  let Size = 2;
}

/// A 2-byte conditional branch instruction.
class MCS6502Inst2ByteRel<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<16> Inst;
  bits<8> imm8; // This name has to match the $name in the args.

  let Inst{15-8} = Opcode.Value;
  let Inst{7-0} = imm8;
  let Size = 2;
  // Together these three imply isConditionalBranch.
  let isBranch = 1;
  let isBarrier = 0;
  let isIndirectBranch = 0;
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

/// A 3-byte store-to-memory instruction.
class MCS6502Inst3ByteStore<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<24> Inst; // max size 3 bytes
  bits<16> addr16;
  
  let Inst{23-16} = Opcode.Value;
  let Inst{15-8} = addr16{7-0};
  let Inst{7-0} = addr16{15-8};
  let Size = 3;
  let mayStore = 1;
  // Technically a store-indirect is also mayLoad.
}

/// A 3-byte load-from-memory instruction.
class MCS6502Inst3ByteLoad<dag outs, MCS6502Opcode opcode> :
    MCS6502Inst<outs, opcode> {
  field bits<24> Inst; // max size 3 bytes
  bits<16> addr16;
  
  let Inst{23-16} = Opcode.Value;
  let Inst{15-8} = addr16{7-0};
  let Inst{7-0} = addr16{15-8};
  let Size = 3;
  let mayLoad = 1;
}

/// A 3-byte jump instruction.
class MCS6502Inst3ByteJump<dag outs, MCS6502Opcode opcode, int jsr,
    int ind = 0> : MCS6502Inst<outs, opcode> {
  field bits<24> Inst; // max size 3 bytes
  bits<16> addr16;
  
  let Inst{23-16} = Opcode.Value;
  let Inst{15-8} = addr16{7-0};
  let Inst{7-0} = addr16{15-8};
  let Size = 3;
  let isBranch = 1;
  let isBarrier = 1;  // always transfers control flow to another block
  let isIndirectBranch = ind;

  let isCall = jsr;
}

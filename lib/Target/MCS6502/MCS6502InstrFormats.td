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
  let Inst{15-8} = Arg{7-0};
  let Inst{7-0} = Arg{15-8};
  let Size = 3;
}

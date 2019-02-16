//===-- MCS6502InstrInfo.td - MCS6502 Instructions ---------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

include "MCS6502InstrFormats.td"

defm ORA : alu_inst<"ORA", 0b000>;
defm AND : alu_inst<"AND", 0b001>;
defm EOR : alu_inst<"EOR", 0b010>;
defm ADC : alu_inst<"ADC", 0b011>;
defm LDA : alu_inst<"LDA", 0b101>;
defm CMP : alu_inst<"CMP", 0b110>;
defm SBC : alu_inst<"SBC", 0b111>;

defm STA : sta_inst<"STA", 0b100>;

defm ASL : shift_inst<"ASL", 0b000>;
defm ROL : shift_inst<"ROL", 0b001>;
defm LSR : shift_inst<"LSR", 0b010>;
defm ROR : shift_inst<"ROR", 0b011>;

defm DEC : inc_dec_inst<"DEC", 0b110>;
defm INC : inc_dec_inst<"INC", 0b111>;

defm STX : stx_inst<"STX", 0b100>;
defm LDX : ldx_inst<"LDX", 0b101>;

defm BIT : bit_inst<"BIT", 0b001>;

def JMP_ABS : Inst3Byte<"JMP", 0x4C, MODE_ABS, (outs)>;
def JMP_IND : Inst3Byte<"JMP", 0x6C, MODE_IND, (outs)>;

defm STY : sty_inst<"STY", 0b100>;
defm LDY : ldy_inst<"LDY", 0b101>;

defm CPY : cpx_cpy_inst<"CPY", 0b110>;
defm CPX : cpx_cpy_inst<"CPX", 0b111>;

def BPL : Inst2Byte<"BPL", 0x10, MODE_REL, (outs)>;
def BMI : Inst2Byte<"BMI", 0x30, MODE_REL, (outs)>;
def BVC : Inst2Byte<"BVC", 0x50, MODE_REL, (outs)>;
def BVS : Inst2Byte<"BVS", 0x70, MODE_REL, (outs)>;
def BCC : Inst2Byte<"BCC", 0x90, MODE_REL, (outs)>;
def BCS : Inst2Byte<"BCS", 0xB0, MODE_REL, (outs)>;
def BNE : Inst2Byte<"BNE", 0xD0, MODE_REL, (outs)>;
def BEQ : Inst2Byte<"BEQ", 0xF0, MODE_REL, (outs)>;

def JSR : Inst3Byte<"JSR", 0x20, MODE_ABS, (outs)>;

def BRK : imp_inst<"BRK", 0x00>;
def RTI : imp_inst<"RTI", 0x40>;
def RTS : imp_inst<"RTS", 0x60>;

def PHP : imp_inst<"PHP", 0x08>;
def PLP : imp_inst<"PLP", 0x28>;
def PHA : imp_inst<"PHA", 0x48>;
def PLA : imp_inst<"PLA", 0x68>;
def DEY : imp_inst<"DEY", 0x88>;
def TAY : imp_inst<"TAY", 0xA8>;
def INY : imp_inst<"INY", 0xC8>;
def INX : imp_inst<"INX", 0xE8>;

def CLC : imp_inst<"CLC", 0x18>;
def SEC : imp_inst<"SEC", 0x38>;
def CLI : imp_inst<"CLI", 0x58>;
def SEI : imp_inst<"SEI", 0x78>;
def TYA : imp_inst<"TYA", 0x98>;
def CLV : imp_inst<"CLV", 0xB8>;
def CLD : imp_inst<"CLD", 0xD8>;
def SED : imp_inst<"SED", 0xF8>;

def TXA : imp_inst<"TXA", 0x8A>;
def TXS : imp_inst<"TXA", 0x9A>;
def TAX : imp_inst<"TXA", 0xAA>;
def TSX : imp_inst<"TXA", 0xBA>;
def DEX : imp_inst<"TXA", 0xCA>;
def NOP : imp_inst<"TXA", 0xEA>;

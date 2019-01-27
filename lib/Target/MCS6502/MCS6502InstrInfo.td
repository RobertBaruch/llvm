//=== MCS6502InstrInfo.td - Target Description for MCS6502 -*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the MSC6502 instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "MCS6502InstrFormats.td"

//===----------------------------------------------------------------------===//
// Instructions
//===----------------------------------------------------------------------===//

// Apparently the text must be lowercase, otherwise mnemonic won't match :(

def OPC_ADC_IMM      : MCS6502Opcode<0x69, "adc", MODE_IMM>;
def OPC_ADC_ZP       : MCS6502Opcode<0x65, "adc", MODE_ZP>;
def OPC_ADC_ZPX      : MCS6502Opcode<0x75, "adc", MODE_ZPX>;
def OPC_ADC_ABS      : MCS6502Opcode<0x6D, "adc", MODE_ABS>;
def OPC_ADC_ABSX     : MCS6502Opcode<0x7D, "adc", MODE_ABSX>;
def OPC_ADC_ABSY     : MCS6502Opcode<0x79, "adc", MODE_ABSY>;
def OPC_ADC_INDX     : MCS6502Opcode<0x61, "adc", MODE_INDX>;
def OPC_ADC_INDY     : MCS6502Opcode<0x71, "adc", MODE_INDY>;

def OPC_AND_IMM      : MCS6502Opcode<0x29, "and", MODE_IMM>;
def OPC_AND_ZP       : MCS6502Opcode<0x25, "and", MODE_ZP>;
def OPC_AND_ZPX      : MCS6502Opcode<0x35, "and", MODE_ZPX>;
def OPC_AND_ABS      : MCS6502Opcode<0x2D, "and", MODE_ABS>;
def OPC_AND_ABSX     : MCS6502Opcode<0x3D, "and", MODE_ABSX>;
def OPC_AND_ABSY     : MCS6502Opcode<0x39, "and", MODE_ABSY>;
def OPC_AND_INDX     : MCS6502Opcode<0x21, "and", MODE_INDX>;
def OPC_AND_INDY     : MCS6502Opcode<0x31, "and", MODE_INDY>;

def OPC_ASL_ACC      : MCS6502Opcode<0x0A, "asl", MODE_ACC>;
def OPC_ASL_ZP       : MCS6502Opcode<0x06, "asl", MODE_ZP>;
def OPC_ASL_ZPX      : MCS6502Opcode<0x16, "asl", MODE_ZPX>;
def OPC_ASL_ABS      : MCS6502Opcode<0x0E, "asl", MODE_ABS>;
def OPC_ASL_ABSX     : MCS6502Opcode<0x1E, "asl", MODE_ABSX>;

def OPC_BCC_REL      : MCS6502Opcode<0x90, "bcc", MODE_REL>;
def OPC_BCS_REL      : MCS6502Opcode<0xB0, "bcs", MODE_REL>;
def OPC_BEQ_REL      : MCS6502Opcode<0xF0, "beq", MODE_REL>;

def OPC_BIT_ZP       : MCS6502Opcode<0x24, "bit", MODE_ZP>;
def OPC_BIT_ABS      : MCS6502Opcode<0x2C, "bit", MODE_ABS>;

def OPC_BMI_REL      : MCS6502Opcode<0x30, "bmi", MODE_REL>;
def OPC_BNE_REL      : MCS6502Opcode<0xD0, "bne", MODE_REL>;
def OPC_BPL_REL      : MCS6502Opcode<0x10, "bpl", MODE_REL>;

def OPC_BRK          : MCS6502Opcode<0x00, "brk", MODE_IMP>;

def OPC_BVC_REL      : MCS6502Opcode<0x50, "bvc", MODE_REL>;
def OPC_BVS_REL      : MCS6502Opcode<0x70, "bvs", MODE_REL>;

def OPC_CLC          : MCS6502Opcode<0x18, "clc", MODE_IMP>;
def OPC_CLD          : MCS6502Opcode<0xD8, "cld", MODE_IMP>;
def OPC_CLI          : MCS6502Opcode<0x58, "cli", MODE_IMP>;
def OPC_CLV          : MCS6502Opcode<0xB8, "clv", MODE_IMP>;

def OPC_CMP_IMM      : MCS6502Opcode<0xC9, "cmp", MODE_IMM>;
def OPC_CMP_ZP       : MCS6502Opcode<0xC5, "cmp", MODE_ZP>;
def OPC_CMP_ZPX      : MCS6502Opcode<0xD5, "cmp", MODE_ZPX>;
def OPC_CMP_ABS      : MCS6502Opcode<0xCD, "cmp", MODE_ABS>;
def OPC_CMP_ABSX     : MCS6502Opcode<0xDD, "cmp", MODE_ABSX>;
def OPC_CMP_ABSY     : MCS6502Opcode<0xD9, "cmp", MODE_ABSY>;
def OPC_CMP_INDX     : MCS6502Opcode<0xC1, "cmp", MODE_INDX>;
def OPC_CMP_INDY     : MCS6502Opcode<0xD1, "cmp", MODE_INDY>;

def OPC_CPX_IMM      : MCS6502Opcode<0xE0, "cpx", MODE_IMM>;
def OPC_CPX_ZP       : MCS6502Opcode<0xE4, "cpx", MODE_ZP>;
def OPC_CPX_ABS      : MCS6502Opcode<0xEC, "cpx", MODE_ABS>;

def OPC_CPY_IMM      : MCS6502Opcode<0xC0, "cpy", MODE_IMM>;
def OPC_CPY_ZP       : MCS6502Opcode<0xC4, "cpy", MODE_ZP>;
def OPC_CPY_ABS      : MCS6502Opcode<0xCC, "cpy", MODE_ABS>;

def OPC_DEC_ZP       : MCS6502Opcode<0xC6, "dec", MODE_ZP>;
def OPC_DEC_ZPX      : MCS6502Opcode<0xD6, "dec", MODE_ZPX>;
def OPC_DEC_ABS      : MCS6502Opcode<0xCE, "dec", MODE_ABS>;
def OPC_DEC_ABSX     : MCS6502Opcode<0xDE, "dec", MODE_ABSX>;

def OPC_DEX          : MCS6502Opcode<0xCA, "dex", MODE_IMP>;
def OPC_DEY          : MCS6502Opcode<0x88, "dey", MODE_IMP>;

def OPC_EOR_IMM      : MCS6502Opcode<0x49, "eor", MODE_IMM>;
def OPC_EOR_ZP       : MCS6502Opcode<0x45, "eor", MODE_ZP>;
def OPC_EOR_ZPX      : MCS6502Opcode<0x55, "eor", MODE_ZPX>;
def OPC_EOR_ABS      : MCS6502Opcode<0x4D, "eor", MODE_ABS>;
def OPC_EOR_ABSX     : MCS6502Opcode<0x5D, "eor", MODE_ABSX>;
def OPC_EOR_ABSY     : MCS6502Opcode<0x59, "eor", MODE_ABSY>;
def OPC_EOR_INDX     : MCS6502Opcode<0x41, "eor", MODE_INDX>;
def OPC_EOR_INDY     : MCS6502Opcode<0x51, "eor", MODE_INDY>;

def OPC_INC_ZP       : MCS6502Opcode<0xE6, "inc", MODE_ZP>;
def OPC_INC_ZPX      : MCS6502Opcode<0xF6, "inc", MODE_ZPX>;
def OPC_INC_ABS      : MCS6502Opcode<0xEE, "inc", MODE_ABS>;
def OPC_INC_ABSX     : MCS6502Opcode<0xFE, "inc", MODE_ABSX>;

def OPC_INX          : MCS6502Opcode<0xE8, "inx", MODE_IMP>;
def OPC_INY          : MCS6502Opcode<0xC8, "iny", MODE_IMP>;

def OPC_JMP_ABS      : MCS6502Opcode<0x4C, "jmp", MODE_ABS>;
def OPC_JMP_IND      : MCS6502Opcode<0x6C, "jmp", MODE_IND>;
def OPC_JSR_ABS      : MCS6502Opcode<0x20, "jsr", MODE_ABS>;

def OPC_LDA_IMM      : MCS6502Opcode<0xA9, "lda", MODE_IMM>;
def OPC_LDA_ZP       : MCS6502Opcode<0xA5, "lda", MODE_ZP>;
def OPC_LDA_ZPX      : MCS6502Opcode<0xB5, "lda", MODE_ZPX>;
def OPC_LDA_ABS      : MCS6502Opcode<0xAD, "lda", MODE_ABS>;
def OPC_LDA_ABSX     : MCS6502Opcode<0xBD, "lda", MODE_ABSX>;
def OPC_LDA_ABSY     : MCS6502Opcode<0xB9, "lda", MODE_ABSY>;
def OPC_LDA_INDX     : MCS6502Opcode<0xA1, "lda", MODE_INDX>;
def OPC_LDA_INDY     : MCS6502Opcode<0xB1, "lda", MODE_INDY>;

def OPC_LDX_IMM      : MCS6502Opcode<0xA2, "ldx", MODE_IMM>;
def OPC_LDX_ZP       : MCS6502Opcode<0xA6, "ldx", MODE_ZP>;
def OPC_LDX_ZPY      : MCS6502Opcode<0xB6, "ldx", MODE_ZPY>;
def OPC_LDX_ABS      : MCS6502Opcode<0xAE, "ldx", MODE_ABS>;
def OPC_LDX_ABSY     : MCS6502Opcode<0xBE, "ldx", MODE_ABSY>;

def OPC_LDY_IMM      : MCS6502Opcode<0xA0, "ldy", MODE_IMM>;
def OPC_LDY_ZP       : MCS6502Opcode<0xA4, "ldy", MODE_ZP>;
def OPC_LDY_ZPX      : MCS6502Opcode<0xB4, "ldy", MODE_ZPX>;
def OPC_LDY_ABS      : MCS6502Opcode<0xAC, "ldy", MODE_ABS>;
def OPC_LDY_ABSX     : MCS6502Opcode<0xBC, "ldy", MODE_ABSX>;

def OPC_LSR_ACC      : MCS6502Opcode<0x4A, "lsr", MODE_ACC>;
def OPC_LSR_ZP       : MCS6502Opcode<0x46, "lsr", MODE_ZP>;
def OPC_LSR_ZPX      : MCS6502Opcode<0x56, "lsr", MODE_ZPX>;
def OPC_LSR_ABS      : MCS6502Opcode<0x4E, "lsr", MODE_ABS>;
def OPC_LSR_ABSX     : MCS6502Opcode<0x5E, "lsr", MODE_ABSX>;

def OPC_NOP          : MCS6502Opcode<0xEA, "nop", MODE_IMP>;

def OPC_ORA_IMM      : MCS6502Opcode<0x09, "ora", MODE_IMM>;
def OPC_ORA_ZP       : MCS6502Opcode<0x05, "ora", MODE_ZP>;
def OPC_ORA_ZPX      : MCS6502Opcode<0x15, "ora", MODE_ZPX>;
def OPC_ORA_ABS      : MCS6502Opcode<0x0D, "ora", MODE_ABS>;
def OPC_ORA_ABSX     : MCS6502Opcode<0x1D, "ora", MODE_ABSX>;
def OPC_ORA_ABSY     : MCS6502Opcode<0x19, "ora", MODE_ABSY>;
def OPC_ORA_INDX     : MCS6502Opcode<0x01, "ora", MODE_INDX>;
def OPC_ORA_INDY     : MCS6502Opcode<0x11, "ora", MODE_INDY>;

def OPC_PHA          : MCS6502Opcode<0x48, "pha", MODE_IMP>;
def OPC_PHP          : MCS6502Opcode<0x08, "php", MODE_IMP>;
def OPC_PLA          : MCS6502Opcode<0x68, "pla", MODE_IMP>;
def OPC_PLP          : MCS6502Opcode<0x28, "plp", MODE_IMP>;

def OPC_ROL_ACC      : MCS6502Opcode<0x2A, "rol", MODE_ACC>;
def OPC_ROL_ZP       : MCS6502Opcode<0x26, "rol", MODE_ZP>;
def OPC_ROL_ZPX      : MCS6502Opcode<0x36, "rol", MODE_ZPX>;
def OPC_ROL_ABS      : MCS6502Opcode<0x2E, "rol", MODE_ABS>;
def OPC_ROL_ABSX     : MCS6502Opcode<0x3E, "rol", MODE_ABSX>;

def OPC_ROR_ACC      : MCS6502Opcode<0x6A, "ror", MODE_ACC>;
def OPC_ROR_ZP       : MCS6502Opcode<0x66, "ror", MODE_ZP>;
def OPC_ROR_ZPX      : MCS6502Opcode<0x76, "ror", MODE_ZPX>;
def OPC_ROR_ABS      : MCS6502Opcode<0x6E, "ror", MODE_ABS>;
def OPC_ROR_ABSX     : MCS6502Opcode<0x7E, "ror", MODE_ABSX>;

def OPC_RTI          : MCS6502Opcode<0x40, "rti", MODE_IMP>;
def OPC_RTS          : MCS6502Opcode<0x60, "rts", MODE_IMP>;

def OPC_SBC_IMM      : MCS6502Opcode<0xE9, "sbc", MODE_IMM>;
def OPC_SBC_ZP       : MCS6502Opcode<0xE5, "sbc", MODE_ZP>;
def OPC_SBC_ZPX      : MCS6502Opcode<0xF5, "sbc", MODE_ZPX>;
def OPC_SBC_ABS      : MCS6502Opcode<0xED, "sbc", MODE_ABS>;
def OPC_SBC_ABSX     : MCS6502Opcode<0xFD, "sbc", MODE_ABSX>;
def OPC_SBC_ABSY     : MCS6502Opcode<0xF9, "sbc", MODE_ABSY>;
def OPC_SBC_INDX     : MCS6502Opcode<0xE1, "sbc", MODE_INDX>;
def OPC_SBC_INDY     : MCS6502Opcode<0xF1, "sbc", MODE_INDY>;

def OPC_SEC          : MCS6502Opcode<0x38, "sec", MODE_IMP>;
def OPC_SED          : MCS6502Opcode<0xF8, "sed", MODE_IMP>;
def OPC_SEI          : MCS6502Opcode<0x78, "sei", MODE_IMP>;

def OPC_STA_ZP       : MCS6502Opcode<0x85, "sta", MODE_ZP>;
def OPC_STA_ZPX      : MCS6502Opcode<0x95, "sta", MODE_ZPX>;
def OPC_STA_ABS      : MCS6502Opcode<0x8D, "sta", MODE_ABS>;
def OPC_STA_ABSX     : MCS6502Opcode<0x9D, "sta", MODE_ABSX>;
def OPC_STA_ABSY     : MCS6502Opcode<0x99, "sta", MODE_ABSY>;
def OPC_STA_INDX     : MCS6502Opcode<0x81, "sta", MODE_INDX>;
def OPC_STA_INDY     : MCS6502Opcode<0x91, "sta", MODE_INDY>;

def OPC_STX_ZP       : MCS6502Opcode<0x86, "stx", MODE_ZP>;
def OPC_STX_ZPY      : MCS6502Opcode<0x96, "stx", MODE_ZPY>;
def OPC_STX_ABS      : MCS6502Opcode<0x8E, "stx", MODE_ABS>;

def OPC_STY_ZP       : MCS6502Opcode<0x84, "sty", MODE_ZP>;
def OPC_STY_ZPX      : MCS6502Opcode<0x94, "sty", MODE_ZPX>;
def OPC_STY_ABS      : MCS6502Opcode<0x8C, "sty", MODE_ABS>;

def OPC_TAX          : MCS6502Opcode<0xAA, "tax", MODE_IMP>;
def OPC_TAY          : MCS6502Opcode<0xA8, "tay", MODE_IMP>;
def OPC_TSX          : MCS6502Opcode<0xBA, "tsx", MODE_IMP>;
def OPC_TXA          : MCS6502Opcode<0x8A, "txa", MODE_IMP>;
def OPC_TXS          : MCS6502Opcode<0x9A, "txs", MODE_IMP>;
def OPC_TYA          : MCS6502Opcode<0x98, "tya", MODE_IMP>;

def BRK_IMP  : MCS6502Inst1Byte<(outs), OPC_BRK>;
def CLC_IMP  : MCS6502Inst1Byte<(outs), OPC_CLC>;
def CLD_IMP  : MCS6502Inst1Byte<(outs), OPC_CLD>;
def CLI_IMP  : MCS6502Inst1Byte<(outs), OPC_CLI>;
def CLV_IMP  : MCS6502Inst1Byte<(outs), OPC_CLV>;
def DEX_IMP  : MCS6502Inst1Byte<(outs), OPC_DEX>;
def DEY_IMP  : MCS6502Inst1Byte<(outs), OPC_DEY>;
def INX_IMP  : MCS6502Inst1Byte<(outs), OPC_INX>;
def INY_IMP  : MCS6502Inst1Byte<(outs), OPC_INY>;
def NOP_IMP  : MCS6502Inst1Byte<(outs), OPC_NOP>;
def PHA_IMP  : MCS6502Inst1Byte<(outs), OPC_PHA>;
def PHP_IMP  : MCS6502Inst1Byte<(outs), OPC_PHP>;
def PLA_IMP  : MCS6502Inst1Byte<(outs), OPC_PLA>;
def PLP_IMP  : MCS6502Inst1Byte<(outs), OPC_PLP>;
def RTI_IMP  : MCS6502Inst1Byte<(outs), OPC_RTI>;
def RTS_IMP  : MCS6502Inst1Byte<(outs), OPC_RTS>;
def SEC_IMP  : MCS6502Inst1Byte<(outs), OPC_SEC>;
def SED_IMP  : MCS6502Inst1Byte<(outs), OPC_SED>;
def SEI_IMP  : MCS6502Inst1Byte<(outs), OPC_SEI>;
def TAX_IMP  : MCS6502Inst1Byte<(outs), OPC_TAX>;
def TAY_IMP  : MCS6502Inst1Byte<(outs), OPC_TAY>;
def TSX_IMP  : MCS6502Inst1Byte<(outs), OPC_TSX>;
def TXA_IMP  : MCS6502Inst1Byte<(outs), OPC_TXA>;
def TXS_IMP  : MCS6502Inst1Byte<(outs), OPC_TXS>;
def TYA_IMP  : MCS6502Inst1Byte<(outs), OPC_TYA>;

def ADC_IMM  : MCS6502Inst2Byte<(outs), OPC_ADC_IMM>;
def ADC_ZP   : MCS6502Inst2Byte<(outs), OPC_ADC_ZP>;
def ADC_ZPX  : MCS6502Inst2Byte<(outs), OPC_ADC_ZPX>;
def ADC_ABS  : MCS6502Inst3Byte<(outs), OPC_ADC_ABS>;
def ADC_ABSX : MCS6502Inst3Byte<(outs), OPC_ADC_ABSX>;
def ADC_ABSY : MCS6502Inst3Byte<(outs), OPC_ADC_ABSY>;
def ADC_INDX : MCS6502Inst3Byte<(outs), OPC_ADC_INDX>;
def ADC_INDY : MCS6502Inst3Byte<(outs), OPC_ADC_INDY>;

def AND_IMM  : MCS6502Inst2Byte<(outs), OPC_AND_IMM>;
def AND_ZP   : MCS6502Inst2Byte<(outs), OPC_AND_ZP>;
def AND_ZPX  : MCS6502Inst2Byte<(outs), OPC_AND_ZPX>;
def AND_ABS  : MCS6502Inst3Byte<(outs), OPC_AND_ABS>;
def AND_ABSX : MCS6502Inst3Byte<(outs), OPC_AND_ABSX>;
def AND_ABSY : MCS6502Inst3Byte<(outs), OPC_AND_ABSY>;
def AND_INDX : MCS6502Inst3Byte<(outs), OPC_AND_INDX>;
def AND_INDY : MCS6502Inst3Byte<(outs), OPC_AND_INDY>;

def ASL_ACC  : MCS6502Inst1Byte<(outs), OPC_ASL_ACC>;
def ASL_ZP   : MCS6502Inst2Byte<(outs), OPC_ASL_ZP>;
def ASL_ZPX  : MCS6502Inst2Byte<(outs), OPC_ASL_ZPX>;
def ASL_ABS  : MCS6502Inst3Byte<(outs), OPC_ASL_ABS>;
def ASL_ABSX : MCS6502Inst3Byte<(outs), OPC_ASL_ABSX>;

def BCC_REL  : MCS6502Inst2ByteRel<(outs), OPC_BCC_REL>;
def BCS_REL  : MCS6502Inst2ByteRel<(outs), OPC_BCS_REL>;
def BEQ_REL  : MCS6502Inst2ByteRel<(outs), OPC_BEQ_REL>;
def BMI_REL  : MCS6502Inst2ByteRel<(outs), OPC_BMI_REL>;
def BNE_REL  : MCS6502Inst2ByteRel<(outs), OPC_BNE_REL>;
def BPL_REL  : MCS6502Inst2ByteRel<(outs), OPC_BPL_REL>;
def BVC_REL  : MCS6502Inst2ByteRel<(outs), OPC_BVC_REL>;
def BVS_REL  : MCS6502Inst2ByteRel<(outs), OPC_BVS_REL>;

def BIT_ZP   : MCS6502Inst2Byte<(outs), OPC_BIT_ZP>;
def BIT_ABS  : MCS6502Inst3Byte<(outs), OPC_BIT_ABS>;

def CMP_IMM  : MCS6502Inst2Byte<(outs), OPC_CMP_IMM>;
def CMP_ZP   : MCS6502Inst2Byte<(outs), OPC_CMP_ZP>;
def CMP_ZPX  : MCS6502Inst2Byte<(outs), OPC_CMP_ZPX>;
def CMP_ABS  : MCS6502Inst3Byte<(outs), OPC_CMP_ABS>;
def CMP_ABSX : MCS6502Inst3Byte<(outs), OPC_CMP_ABSX>;
def CMP_ABSY : MCS6502Inst3Byte<(outs), OPC_CMP_ABSY>;
def CMP_INDX : MCS6502Inst3Byte<(outs), OPC_CMP_INDX>;
def CMP_INDY : MCS6502Inst3Byte<(outs), OPC_CMP_INDY>;

def EOR_IMM  : MCS6502Inst2Byte<(outs), OPC_EOR_IMM>;
def EOR_ZP   : MCS6502Inst2Byte<(outs), OPC_EOR_ZP>;
def EOR_ZPX  : MCS6502Inst2Byte<(outs), OPC_EOR_ZPX>;
def EOR_ABS  : MCS6502Inst3Byte<(outs), OPC_EOR_ABS>;
def EOR_ABSX : MCS6502Inst3Byte<(outs), OPC_EOR_ABSX>;
def EOR_ABSY : MCS6502Inst3Byte<(outs), OPC_EOR_ABSY>;
def EOR_INDX : MCS6502Inst3Byte<(outs), OPC_EOR_INDX>;
def EOR_INDY : MCS6502Inst3Byte<(outs), OPC_EOR_INDY>;

def LDA_IMM  : MCS6502Inst2Byte<(outs), OPC_LDA_IMM>;
def LDA_ZP   : MCS6502Inst2Byte<(outs), OPC_LDA_ZP>;
def LDA_ZPX  : MCS6502Inst2Byte<(outs), OPC_LDA_ZPX>;
def LDA_ABS  : MCS6502Inst3Byte<(outs), OPC_LDA_ABS>;
def LDA_ABSX : MCS6502Inst3Byte<(outs), OPC_LDA_ABSX>;
def LDA_ABSY : MCS6502Inst3Byte<(outs), OPC_LDA_ABSY>;
def LDA_INDX : MCS6502Inst3Byte<(outs), OPC_LDA_INDX>;
def LDA_INDY : MCS6502Inst3Byte<(outs), OPC_LDA_INDY>;

def ORA_IMM  : MCS6502Inst2Byte<(outs), OPC_ORA_IMM>;
def ORA_ZP   : MCS6502Inst2Byte<(outs), OPC_ORA_ZP>;
def ORA_ZPX  : MCS6502Inst2Byte<(outs), OPC_ORA_ZPX>;
def ORA_ABS  : MCS6502Inst3Byte<(outs), OPC_ORA_ABS>;
def ORA_ABSX : MCS6502Inst3Byte<(outs), OPC_ORA_ABSX>;
def ORA_ABSY : MCS6502Inst3Byte<(outs), OPC_ORA_ABSY>;
def ORA_INDX : MCS6502Inst3Byte<(outs), OPC_ORA_INDX>;
def ORA_INDY : MCS6502Inst3Byte<(outs), OPC_ORA_INDY>;

def SBC_IMM  : MCS6502Inst2Byte<(outs), OPC_SBC_IMM>;
def SBC_ZP   : MCS6502Inst2Byte<(outs), OPC_SBC_ZP>;
def SBC_ZPX  : MCS6502Inst2Byte<(outs), OPC_SBC_ZPX>;
def SBC_ABS  : MCS6502Inst3Byte<(outs), OPC_SBC_ABS>;
def SBC_ABSX : MCS6502Inst3Byte<(outs), OPC_SBC_ABSX>;
def SBC_ABSY : MCS6502Inst3Byte<(outs), OPC_SBC_ABSY>;
def SBC_INDX : MCS6502Inst3Byte<(outs), OPC_SBC_INDX>;
def SBC_INDY : MCS6502Inst3Byte<(outs), OPC_SBC_INDY>;

def STA_ZP   : MCS6502Inst2Byte<(outs), OPC_STA_ZP>;
def STA_ZPX  : MCS6502Inst2Byte<(outs), OPC_STA_ZPX>;
def STA_ABS  : MCS6502Inst3Byte<(outs), OPC_STA_ABS>;
def STA_ABSX : MCS6502Inst3Byte<(outs), OPC_STA_ABSX>;
def STA_ABSY : MCS6502Inst3Byte<(outs), OPC_STA_ABSY>;
def STA_INDX : MCS6502Inst3Byte<(outs), OPC_STA_INDX>;
def STA_INDY : MCS6502Inst3Byte<(outs), OPC_STA_INDY>;

def CPX_IMM  : MCS6502Inst2Byte<(outs), OPC_CPX_IMM>;
def CPX_ZP   : MCS6502Inst2Byte<(outs), OPC_CPX_ZP>;
def CPX_ABS  : MCS6502Inst3Byte<(outs), OPC_CPX_ABS>;

def CPY_IMM  : MCS6502Inst2Byte<(outs), OPC_CPY_IMM>;
def CPY_ZP   : MCS6502Inst2Byte<(outs), OPC_CPY_ZP>;
def CPY_ABS  : MCS6502Inst3Byte<(outs), OPC_CPY_ABS>;

def DEC_ZP   : MCS6502Inst2Byte<(outs), OPC_DEC_ZP>;
def DEC_ZPX  : MCS6502Inst2Byte<(outs), OPC_DEC_ZPX>;
def DEC_ABS  : MCS6502Inst3Byte<(outs), OPC_DEC_ABS>;
def DEC_ABSX : MCS6502Inst3Byte<(outs), OPC_DEC_ABSX>;

def INC_ZP   : MCS6502Inst2Byte<(outs), OPC_INC_ZP>;
def INC_ZPX  : MCS6502Inst2Byte<(outs), OPC_INC_ZPX>;
def INC_ABS  : MCS6502Inst3Byte<(outs), OPC_INC_ABS>;
def INC_ABSX : MCS6502Inst3Byte<(outs), OPC_INC_ABSX>;

def JMP_ABS  : MCS6502Inst3Byte<(outs), OPC_JMP_ABS>;
def JMP_IND  : MCS6502Inst3Byte<(outs), OPC_JMP_IND>;
def JSR_ABS  : MCS6502Inst3Byte<(outs), OPC_JSR_ABS>;

def LDX_IMM  : MCS6502Inst2Byte<(outs), OPC_LDX_IMM>;
def LDX_ZP   : MCS6502Inst2Byte<(outs), OPC_LDX_ZP>;
def LDX_ZPY  : MCS6502Inst2Byte<(outs), OPC_LDX_ZPY>;
def LDX_ABS  : MCS6502Inst3Byte<(outs), OPC_LDX_ABS>;
def LDX_ABSY : MCS6502Inst3Byte<(outs), OPC_LDX_ABSY>;

def LDY_IMM  : MCS6502Inst2Byte<(outs), OPC_LDY_IMM>;
def LDY_ZP   : MCS6502Inst2Byte<(outs), OPC_LDY_ZP>;
def LDY_ZPX  : MCS6502Inst2Byte<(outs), OPC_LDY_ZPX>;
def LDY_ABS  : MCS6502Inst3Byte<(outs), OPC_LDY_ABS>;
def LDY_ABSX : MCS6502Inst3Byte<(outs), OPC_LDY_ABSX>;

def STX_ZP   : MCS6502Inst2Byte<(outs), OPC_STX_ZP>;
def STX_ZPY  : MCS6502Inst2Byte<(outs), OPC_STX_ZPY>;
def STX_ABS  : MCS6502Inst3Byte<(outs), OPC_STX_ABS>;

def STY_ZP   : MCS6502Inst2Byte<(outs), OPC_STY_ZP>;
def STY_ZPX  : MCS6502Inst2Byte<(outs), OPC_STY_ZPX>;
def STY_ABS  : MCS6502Inst3Byte<(outs), OPC_STY_ABS>;

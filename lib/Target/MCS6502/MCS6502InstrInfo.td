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

def OPC_ADC_IMM      : MCS6502Opcode<0x69, "ADC", MODE_IMM>;
def OPC_ADC_ZP       : MCS6502Opcode<0x65, "ADC", MODE_ZP>;
def OPC_ADC_ZPX      : MCS6502Opcode<0x75, "ADC", MODE_ZPX>;
def OPC_ADC_ABS      : MCS6502Opcode<0x6D, "ADC", MODE_ABS>;
def OPC_ADC_ABSX     : MCS6502Opcode<0x7D, "ADC", MODE_ABSX>;
def OPC_ADC_ABSY     : MCS6502Opcode<0x79, "ADC", MODE_ABSY>;
def OPC_ADC_INDX     : MCS6502Opcode<0x61, "ADC", MODE_INDX>;
def OPC_ADC_INDY     : MCS6502Opcode<0x71, "ADC", MODE_INDY>;

def OPC_AND_IMM      : MCS6502Opcode<0x29, "AND", MODE_IMM>;
def OPC_AND_ZP       : MCS6502Opcode<0x25, "AND", MODE_ZP>;
def OPC_AND_ZPX      : MCS6502Opcode<0x35, "AND", MODE_ZPX>;
def OPC_AND_ABS      : MCS6502Opcode<0x2D, "AND", MODE_ABS>;
def OPC_AND_ABSX     : MCS6502Opcode<0x3D, "AND", MODE_ABSX>;
def OPC_AND_ABSY     : MCS6502Opcode<0x39, "AND", MODE_ABSY>;
def OPC_AND_INDX     : MCS6502Opcode<0x21, "AND", MODE_INDX>;
def OPC_AND_INDY     : MCS6502Opcode<0x31, "AND", MODE_INDY>;

def OPC_ASL_ACC      : MCS6502Opcode<0x0A, "ASL", MODE_ACC>;
def OPC_ASL_ZP       : MCS6502Opcode<0x06, "ASL", MODE_ZP>;
def OPC_ASL_ZPX      : MCS6502Opcode<0x16, "ASL", MODE_ZPX>;
def OPC_ASL_ABS      : MCS6502Opcode<0x0E, "ASL", MODE_ABS>;
def OPC_ASL_ABSX     : MCS6502Opcode<0x1E, "ASL", MODE_ABSX>;

def OPC_BCC_REL      : MCS6502Opcode<0x90, "BCC", MODE_REL>;
def OPC_BCS_REL      : MCS6502Opcode<0xB0, "BCS", MODE_REL>;
def OPC_BEQ_REL      : MCS6502Opcode<0xF0, "BEQ", MODE_REL>;

def OPC_BIT_ZP       : MCS6502Opcode<0x24, "BIT", MODE_ZP>;
def OPC_BIT_ABS      : MCS6502Opcode<0x2C, "BIT", MODE_ABS>;

def OPC_BMI_REL      : MCS6502Opcode<0x30, "BMI", MODE_REL>;
def OPC_BNE_REL      : MCS6502Opcode<0xD0, "BNE", MODE_REL>;
def OPC_BPL_REL      : MCS6502Opcode<0x10, "BPL", MODE_REL>;

def OPC_BRK          : MCS6502Opcode<0x00, "BRK", MODE_IMP>;

def OPC_BVC_REL      : MCS6502Opcode<0x50, "BVC", MODE_REL>;
def OPC_BVS_REL      : MCS6502Opcode<0x70, "BVS", MODE_REL>;

def OPC_CLC          : MCS6502Opcode<0x18, "CLC", MODE_IMP>;
def OPC_CLD          : MCS6502Opcode<0xD8, "CLD", MODE_IMP>;
def OPC_CLI          : MCS6502Opcode<0x58, "CLI", MODE_IMP>;
def OPC_CLV          : MCS6502Opcode<0xB8, "CLV", MODE_IMP>;

def OPC_CMP_IMM      : MCS6502Opcode<0xC9, "CMP", MODE_IMM>;
def OPC_CMP_ZP       : MCS6502Opcode<0xC5, "CMP", MODE_ZP>;
def OPC_CMP_ZPX      : MCS6502Opcode<0xD5, "CMP", MODE_ZPX>;
def OPC_CMP_ABS      : MCS6502Opcode<0xCD, "CMP", MODE_ABS>;
def OPC_CMP_ABSX     : MCS6502Opcode<0xDD, "CMP", MODE_ABSX>;
def OPC_CMP_ABSY     : MCS6502Opcode<0xD9, "CMP", MODE_ABSY>;
def OPC_CMP_INDX     : MCS6502Opcode<0xC1, "CMP", MODE_INDX>;
def OPC_CMP_INDY     : MCS6502Opcode<0xD1, "CMP", MODE_INDY>;

def OPC_CPX_IMM      : MCS6502Opcode<0xE0, "CPX", MODE_IMM>;
def OPC_CPX_ZP       : MCS6502Opcode<0xE4, "CPX", MODE_ZP>;
def OPC_CPX_ABS      : MCS6502Opcode<0xEC, "CPX", MODE_ABS>;

def OPC_CPY_IMM      : MCS6502Opcode<0xC0, "CPY", MODE_IMM>;
def OPC_CPY_ZP       : MCS6502Opcode<0xC4, "CPY", MODE_ZP>;
def OPC_CPY_ABS      : MCS6502Opcode<0xCC, "CPY", MODE_ABS>;

def OPC_DEC_ZP       : MCS6502Opcode<0xC6, "DEC", MODE_ZP>;
def OPC_DEC_ZPX      : MCS6502Opcode<0xD6, "DEC", MODE_ZPX>;
def OPC_DEC_ABS      : MCS6502Opcode<0xCE, "DEC", MODE_ABS>;
def OPC_DEC_ABSX     : MCS6502Opcode<0xDE, "DEC", MODE_ABSX>;

def OPC_DEX          : MCS6502Opcode<0xCA, "DEX", MODE_IMP>;
def OPC_DEY          : MCS6502Opcode<0x88, "DEY", MODE_IMP>;

def OPC_EOR_IMM      : MCS6502Opcode<0x49, "EOR", MODE_IMM>;
def OPC_EOR_ZP       : MCS6502Opcode<0x45, "EOR", MODE_ZP>;
def OPC_EOR_ZPX      : MCS6502Opcode<0x55, "EOR", MODE_ZPX>;
def OPC_EOR_ABS      : MCS6502Opcode<0x4D, "EOR", MODE_ABS>;
def OPC_EOR_ABSX     : MCS6502Opcode<0x5D, "EOR", MODE_ABSX>;
def OPC_EOR_ABSY     : MCS6502Opcode<0x59, "EOR", MODE_ABSY>;
def OPC_EOR_INDX     : MCS6502Opcode<0x41, "EOR", MODE_INDX>;
def OPC_EOR_INDY     : MCS6502Opcode<0x51, "EOR", MODE_INDY>;

def OPC_INC_ZP       : MCS6502Opcode<0xE6, "INC", MODE_ZP>;
def OPC_INC_ZPX      : MCS6502Opcode<0xF6, "INC", MODE_ZPX>;
def OPC_INC_ABS      : MCS6502Opcode<0xEE, "INC", MODE_ABS>;
def OPC_INC_ABSX     : MCS6502Opcode<0xFE, "INC", MODE_ABSX>;

def OPC_INX          : MCS6502Opcode<0xE8, "INX", MODE_IMP>;
def OPC_INY          : MCS6502Opcode<0xC8, "INY", MODE_IMP>;

def OPC_JMP_ABS      : MCS6502Opcode<0x4C, "JMP", MODE_ABS>;
def OPC_JMP_IND      : MCS6502Opcode<0x6C, "JMP", MODE_IND>;
def OPC_JSR_ABS      : MCS6502Opcode<0x20, "JSR", MODE_ABS>;

def OPC_LDA_IMM      : MCS6502Opcode<0xA9, "LDA", MODE_IMM>;
def OPC_LDA_ZP       : MCS6502Opcode<0xA5, "LDA", MODE_ZP>;
def OPC_LDA_ZPX      : MCS6502Opcode<0xB5, "LDA", MODE_ZPX>;
def OPC_LDA_ABS      : MCS6502Opcode<0xAD, "LDA", MODE_ABS>;
def OPC_LDA_ABSX     : MCS6502Opcode<0xBD, "LDA", MODE_ABSX>;
def OPC_LDA_ABSY     : MCS6502Opcode<0xB9, "LDA", MODE_ABSY>;
def OPC_LDA_INDX     : MCS6502Opcode<0xA1, "LDA", MODE_INDX>;
def OPC_LDA_INDY     : MCS6502Opcode<0xB1, "LDA", MODE_INDY>;

def OPC_LDX_IMM      : MCS6502Opcode<0xA2, "LDX", MODE_IMM>;
def OPC_LDX_ZP       : MCS6502Opcode<0xA6, "LDX", MODE_ZP>;
def OPC_LDX_ZPY      : MCS6502Opcode<0xB6, "LDX", MODE_ZPY>;
def OPC_LDX_ABS      : MCS6502Opcode<0xAE, "LDX", MODE_ABS>;
def OPC_LDX_ABSY     : MCS6502Opcode<0xBE, "LDX", MODE_ABSY>;

def OPC_LDY_IMM      : MCS6502Opcode<0xA0, "LDY", MODE_IMM>;
def OPC_LDY_ZP       : MCS6502Opcode<0xA4, "LDY", MODE_ZP>;
def OPC_LDY_ZPX      : MCS6502Opcode<0xB4, "LDY", MODE_ZPX>;
def OPC_LDY_ABS      : MCS6502Opcode<0xAC, "LDY", MODE_ABS>;
def OPC_LDY_ABSX     : MCS6502Opcode<0xBC, "LDY", MODE_ABSX>;

def OPC_LSR_ACC      : MCS6502Opcode<0x4A, "LSR", MODE_ACC>;
def OPC_LSR_ZP       : MCS6502Opcode<0x46, "LSR", MODE_ZP>;
def OPC_LSR_ZPX      : MCS6502Opcode<0x56, "LSR", MODE_ZPX>;
def OPC_LSR_ABS      : MCS6502Opcode<0x4E, "LSR", MODE_ABS>;
def OPC_LSR_ABSX     : MCS6502Opcode<0x5E, "LSR", MODE_ABSX>;

def OPC_NOP          : MCS6502Opcode<0xEA, "NOP", MODE_IMP>;

def OPC_ORA_IMM      : MCS6502Opcode<0x09, "ORA", MODE_IMM>;
def OPC_ORA_ZP       : MCS6502Opcode<0x05, "ORA", MODE_ZP>;
def OPC_ORA_ZPX      : MCS6502Opcode<0x15, "ORA", MODE_ZPX>;
def OPC_ORA_ABS      : MCS6502Opcode<0x0D, "ORA", MODE_ABS>;
def OPC_ORA_ABSX     : MCS6502Opcode<0x1D, "ORA", MODE_ABSX>;
def OPC_ORA_ABSY     : MCS6502Opcode<0x19, "ORA", MODE_ABSY>;
def OPC_ORA_INDX     : MCS6502Opcode<0x01, "ORA", MODE_INDX>;
def OPC_ORA_INDY     : MCS6502Opcode<0x11, "ORA", MODE_INDY>;

def OPC_PHA          : MCS6502Opcode<0x48, "PHA", MODE_IMP>;
def OPC_PHP          : MCS6502Opcode<0x08, "PHP", MODE_IMP>;
def OPC_PLA          : MCS6502Opcode<0x68, "PLA", MODE_IMP>;
def OPC_PLP          : MCS6502Opcode<0x28, "PLP", MODE_IMP>;

def OPC_ROL_ACC      : MCS6502Opcode<0x2A, "ROL", MODE_ACC>;
def OPC_ROL_ZP       : MCS6502Opcode<0x26, "ROL", MODE_ZP>;
def OPC_ROL_ZPX      : MCS6502Opcode<0x36, "ROL", MODE_ZPX>;
def OPC_ROL_ABS      : MCS6502Opcode<0x2E, "ROL", MODE_ABS>;
def OPC_ROL_ABSX     : MCS6502Opcode<0x3E, "ROL", MODE_ABSX>;

def OPC_ROR_ACC      : MCS6502Opcode<0x6A, "ROR", MODE_ACC>;
def OPC_ROR_ZP       : MCS6502Opcode<0x66, "ROR", MODE_ZP>;
def OPC_ROR_ZPX      : MCS6502Opcode<0x76, "ROR", MODE_ZPX>;
def OPC_ROR_ABS      : MCS6502Opcode<0x6E, "ROR", MODE_ABS>;
def OPC_ROR_ABSX     : MCS6502Opcode<0x7E, "ROR", MODE_ABSX>;

def OPC_RTI          : MCS6502Opcode<0x40, "RTI", MODE_IMP>;
def OPC_RTS          : MCS6502Opcode<0x60, "RTS", MODE_IMP>;

def OPC_SBC_IMM      : MCS6502Opcode<0xE9, "SBC", MODE_IMM>;
def OPC_SBC_ZP       : MCS6502Opcode<0xE5, "SBC", MODE_ZP>;
def OPC_SBC_ZPX      : MCS6502Opcode<0xF5, "SBC", MODE_ZPX>;
def OPC_SBC_ABS      : MCS6502Opcode<0xED, "SBC", MODE_ABS>;
def OPC_SBC_ABSX     : MCS6502Opcode<0xFD, "SBC", MODE_ABSX>;
def OPC_SBC_ABSY     : MCS6502Opcode<0xF9, "SBC", MODE_ABSY>;
def OPC_SBC_INDX     : MCS6502Opcode<0xE1, "SBC", MODE_INDX>;
def OPC_SBC_INDY     : MCS6502Opcode<0xF1, "SBC", MODE_INDY>;

def OPC_SEC          : MCS6502Opcode<0x38, "SEC", MODE_IMP>;
def OPC_SED          : MCS6502Opcode<0xF8, "SED", MODE_IMP>;
def OPC_SEI          : MCS6502Opcode<0x78, "SEI", MODE_IMP>;

def OPC_STA_ZP       : MCS6502Opcode<0x85, "STA", MODE_ZP>;
def OPC_STA_ZPX      : MCS6502Opcode<0x95, "STA", MODE_ZPX>;
def OPC_STA_ABS      : MCS6502Opcode<0x8D, "STA", MODE_ABS>;
def OPC_STA_ABSX     : MCS6502Opcode<0x9D, "STA", MODE_ABSX>;
def OPC_STA_ABSY     : MCS6502Opcode<0x99, "STA", MODE_ABSY>;
def OPC_STA_INDX     : MCS6502Opcode<0x81, "STA", MODE_INDX>;
def OPC_STA_INDY     : MCS6502Opcode<0x91, "STA", MODE_INDY>;

def OPC_STX_ZP       : MCS6502Opcode<0x86, "STX", MODE_ZP>;
def OPC_STX_ZPY      : MCS6502Opcode<0x96, "STX", MODE_ZPY>;
def OPC_STX_ABS      : MCS6502Opcode<0x8E, "STX", MODE_ABS>;

def OPC_STY_ZP       : MCS6502Opcode<0x84, "STY", MODE_ZP>;
def OPC_STY_ZPX      : MCS6502Opcode<0x94, "STY", MODE_ZPX>;
def OPC_STY_ABS      : MCS6502Opcode<0x8C, "STY", MODE_ABS>;

def OPC_TAX          : MCS6502Opcode<0xAA, "TAX", MODE_IMP>;
def OPC_TAY          : MCS6502Opcode<0xA8, "TAY", MODE_IMP>;
def OPC_TSX          : MCS6502Opcode<0xBA, "TSX", MODE_IMP>;
def OPC_TXA          : MCS6502Opcode<0x8A, "TXA", MODE_IMP>;
def OPC_TXS          : MCS6502Opcode<0x9A, "TXS", MODE_IMP>;
def OPC_TYA          : MCS6502Opcode<0x98, "TYA", MODE_IMP>;

def BRK_IMP  : MVC6502Inst1Byte<(outs), OPC_BRK>;
def CLC_IMP  : MVC6502Inst1Byte<(outs), OPC_CLC>;
def CLD_IMP  : MVC6502Inst1Byte<(outs), OPC_CLD>;
def CLI_IMP  : MVC6502Inst1Byte<(outs), OPC_CLI>;
def CLV_IMP  : MVC6502Inst1Byte<(outs), OPC_CLV>;
def DEX_IMP  : MVC6502Inst1Byte<(outs), OPC_DEX>;
def DEY_IMP  : MVC6502Inst1Byte<(outs), OPC_DEY>;
def INX_IMP  : MVC6502Inst1Byte<(outs), OPC_INX>;
def INY_IMP  : MVC6502Inst1Byte<(outs), OPC_INY>;
def NOP_IMP  : MVC6502Inst1Byte<(outs), OPC_NOP>;
def PHA_IMP  : MVC6502Inst1Byte<(outs), OPC_PHA>;
def PHP_IMP  : MVC6502Inst1Byte<(outs), OPC_PHP>;
def PLA_IMP  : MVC6502Inst1Byte<(outs), OPC_PLA>;
def PLP_IMP  : MVC6502Inst1Byte<(outs), OPC_PLP>;
def RTI_IMP  : MVC6502Inst1Byte<(outs), OPC_RTI>;
def RTS_IMP  : MVC6502Inst1Byte<(outs), OPC_RTS>;
def SEC_IMP  : MVC6502Inst1Byte<(outs), OPC_SEC>;
def SED_IMP  : MVC6502Inst1Byte<(outs), OPC_SED>;
def SEI_IMP  : MVC6502Inst1Byte<(outs), OPC_SEI>;
def TAX_IMP  : MVC6502Inst1Byte<(outs), OPC_TAX>;
def TAY_IMP  : MVC6502Inst1Byte<(outs), OPC_TAY>;
def TSX_IMP  : MVC6502Inst1Byte<(outs), OPC_TSX>;
def TXA_IMP  : MVC6502Inst1Byte<(outs), OPC_TXA>;
def TXS_IMP  : MVC6502Inst1Byte<(outs), OPC_TXS>;
def TYA_IMP  : MVC6502Inst1Byte<(outs), OPC_TYA>;

def ADC_IMM  : MVC6502Inst2Byte<(outs), OPC_ADC_IMM>;
def ADC_ZP   : MVC6502Inst2Byte<(outs), OPC_ADC_ZP>;
def ADC_ZPX  : MVC6502Inst2Byte<(outs), OPC_ADC_ZPX>;
def ADC_ABS  : MVC6502Inst3Byte<(outs), OPC_ADC_ABS>;
def ADC_ABSX : MVC6502Inst3Byte<(outs), OPC_ADC_ABSX>;
def ADC_ABSY : MVC6502Inst3Byte<(outs), OPC_ADC_ABSY>;
def ADC_INDX : MVC6502Inst3Byte<(outs), OPC_ADC_INDX>;
def ADC_INDY : MVC6502Inst3Byte<(outs), OPC_ADC_INDY>;

def AND_IMM  : MVC6502Inst2Byte<(outs), OPC_AND_IMM>;
def AND_ZP   : MVC6502Inst2Byte<(outs), OPC_AND_ZP>;
def AND_ZPX  : MVC6502Inst2Byte<(outs), OPC_AND_ZPX>;
def AND_ABS  : MVC6502Inst3Byte<(outs), OPC_AND_ABS>;
def AND_ABSX : MVC6502Inst3Byte<(outs), OPC_AND_ABSX>;
def AND_ABSY : MVC6502Inst3Byte<(outs), OPC_AND_ABSY>;
def AND_INDX : MVC6502Inst3Byte<(outs), OPC_AND_INDX>;
def AND_INDY : MVC6502Inst3Byte<(outs), OPC_AND_INDY>;

def ASL_ACC  : MVC6502Inst1Byte<(outs), OPC_ASL_ACC>;
def ASL_ZP   : MVC6502Inst2Byte<(outs), OPC_ASL_ZP>;
def ASL_ZPX  : MVC6502Inst2Byte<(outs), OPC_ASL_ZPX>;
def ASL_ABS  : MVC6502Inst3Byte<(outs), OPC_ASL_ABS>;
def ASL_ABSX : MVC6502Inst3Byte<(outs), OPC_ASL_ABSX>;

def BCC_REL  : MVC6502Inst2Byte<(outs), OPC_BCC_REL>;
def BCS_REL  : MVC6502Inst2Byte<(outs), OPC_BCS_REL>;
def BEQ_REL  : MVC6502Inst2Byte<(outs), OPC_BEQ_REL>;
def BMI_REL  : MVC6502Inst2Byte<(outs), OPC_BMI_REL>;
def BNE_REL  : MVC6502Inst2Byte<(outs), OPC_BNE_REL>;
def BPL_REL  : MVC6502Inst2Byte<(outs), OPC_BPL_REL>;
def BVC_REL  : MVC6502Inst2Byte<(outs), OPC_BVC_REL>;
def BVS_REL  : MVC6502Inst2Byte<(outs), OPC_BVS_REL>;

def BIT_ZP   : MVC6502Inst2Byte<(outs), OPC_BIT_ZP>;
def BIT_ABS  : MVC6502Inst3Byte<(outs), OPC_BIT_ABS>;

def CMP_IMM  : MVC6502Inst2Byte<(outs), OPC_CMP_IMM>;
def CMP_ZP   : MVC6502Inst2Byte<(outs), OPC_CMP_ZP>;
def CMP_ZPX  : MVC6502Inst2Byte<(outs), OPC_CMP_ZPX>;
def CMP_ABS  : MVC6502Inst3Byte<(outs), OPC_CMP_ABS>;
def CMP_ABSX : MVC6502Inst3Byte<(outs), OPC_CMP_ABSX>;
def CMP_ABSY : MVC6502Inst3Byte<(outs), OPC_CMP_ABSY>;
def CMP_INDX : MVC6502Inst3Byte<(outs), OPC_CMP_INDX>;
def CMP_INDY : MVC6502Inst3Byte<(outs), OPC_CMP_INDY>;

def EOR_IMM  : MVC6502Inst2Byte<(outs), OPC_EOR_IMM>;
def EOR_ZP   : MVC6502Inst2Byte<(outs), OPC_EOR_ZP>;
def EOR_ZPX  : MVC6502Inst2Byte<(outs), OPC_EOR_ZPX>;
def EOR_ABS  : MVC6502Inst3Byte<(outs), OPC_EOR_ABS>;
def EOR_ABSX : MVC6502Inst3Byte<(outs), OPC_EOR_ABSX>;
def EOR_ABSY : MVC6502Inst3Byte<(outs), OPC_EOR_ABSY>;
def EOR_INDX : MVC6502Inst3Byte<(outs), OPC_EOR_INDX>;
def EOR_INDY : MVC6502Inst3Byte<(outs), OPC_EOR_INDY>;

def LDA_IMM  : MVC6502Inst2Byte<(outs), OPC_LDA_IMM>;
def LDA_ZP   : MVC6502Inst2Byte<(outs), OPC_LDA_ZP>;
def LDA_ZPX  : MVC6502Inst2Byte<(outs), OPC_LDA_ZPX>;
def LDA_ABS  : MVC6502Inst3Byte<(outs), OPC_LDA_ABS>;
def LDA_ABSX : MVC6502Inst3Byte<(outs), OPC_LDA_ABSX>;
def LDA_ABSY : MVC6502Inst3Byte<(outs), OPC_LDA_ABSY>;
def LDA_INDX : MVC6502Inst3Byte<(outs), OPC_LDA_INDX>;
def LDA_INDY : MVC6502Inst3Byte<(outs), OPC_LDA_INDY>;

def ORA_IMM  : MVC6502Inst2Byte<(outs), OPC_ORA_IMM>;
def ORA_ZP   : MVC6502Inst2Byte<(outs), OPC_ORA_ZP>;
def ORA_ZPX  : MVC6502Inst2Byte<(outs), OPC_ORA_ZPX>;
def ORA_ABS  : MVC6502Inst3Byte<(outs), OPC_ORA_ABS>;
def ORA_ABSX : MVC6502Inst3Byte<(outs), OPC_ORA_ABSX>;
def ORA_ABSY : MVC6502Inst3Byte<(outs), OPC_ORA_ABSY>;
def ORA_INDX : MVC6502Inst3Byte<(outs), OPC_ORA_INDX>;
def ORA_INDY : MVC6502Inst3Byte<(outs), OPC_ORA_INDY>;

def SBC_IMM  : MVC6502Inst2Byte<(outs), OPC_SBC_IMM>;
def SBC_ZP   : MVC6502Inst2Byte<(outs), OPC_SBC_ZP>;
def SBC_ZPX  : MVC6502Inst2Byte<(outs), OPC_SBC_ZPX>;
def SBC_ABS  : MVC6502Inst3Byte<(outs), OPC_SBC_ABS>;
def SBC_ABSX : MVC6502Inst3Byte<(outs), OPC_SBC_ABSX>;
def SBC_ABSY : MVC6502Inst3Byte<(outs), OPC_SBC_ABSY>;
def SBC_INDX : MVC6502Inst3Byte<(outs), OPC_SBC_INDX>;
def SBC_INDY : MVC6502Inst3Byte<(outs), OPC_SBC_INDY>;

def STA_ZP   : MVC6502Inst2Byte<(outs), OPC_STA_ZP>;
def STA_ZPX  : MVC6502Inst2Byte<(outs), OPC_STA_ZPX>;
def STA_ABS  : MVC6502Inst3Byte<(outs), OPC_STA_ABS>;
def STA_ABSX : MVC6502Inst3Byte<(outs), OPC_STA_ABSX>;
def STA_ABSY : MVC6502Inst3Byte<(outs), OPC_STA_ABSY>;
def STA_INDX : MVC6502Inst3Byte<(outs), OPC_STA_INDX>;
def STA_INDY : MVC6502Inst3Byte<(outs), OPC_STA_INDY>;

def CPX_IMM  : MVC6502Inst2Byte<(outs), OPC_CPX_IMM>;
def CPX_ZP   : MVC6502Inst2Byte<(outs), OPC_CPX_ZP>;
def CPX_ABS  : MVC6502Inst3Byte<(outs), OPC_CPX_ABS>;

def CPY_IMM  : MVC6502Inst2Byte<(outs), OPC_CPY_IMM>;
def CPY_ZP   : MVC6502Inst2Byte<(outs), OPC_CPY_ZP>;
def CPY_ABS  : MVC6502Inst3Byte<(outs), OPC_CPY_ABS>;

def DEC_ZP   : MVC6502Inst2Byte<(outs), OPC_DEC_ZP>;
def DEC_ZPX  : MVC6502Inst2Byte<(outs), OPC_DEC_ZPX>;
def DEC_ABS  : MVC6502Inst3Byte<(outs), OPC_DEC_ABS>;
def DEC_ABSX : MVC6502Inst3Byte<(outs), OPC_DEC_ABSX>;

def INC_ZP   : MVC6502Inst2Byte<(outs), OPC_INC_ZP>;
def INC_ZPX  : MVC6502Inst2Byte<(outs), OPC_INC_ZPX>;
def INC_ABS  : MVC6502Inst3Byte<(outs), OPC_INC_ABS>;
def INC_ABSX : MVC6502Inst3Byte<(outs), OPC_INC_ABSX>;

def JMP_ABS  : MVC6502Inst3Byte<(outs), OPC_JMP_ABS>;
def JMP_IND  : MVC6502Inst3Byte<(outs), OPC_JMP_IND>;
def JSR_ABS  : MVC6502Inst3Byte<(outs), OPC_JSR_ABS>;

def LDX_IMM  : MVC6502Inst2Byte<(outs), OPC_LDX_IMM>;
def LDX_ZP   : MVC6502Inst2Byte<(outs), OPC_LDX_ZP>;
def LDX_ZPY  : MVC6502Inst2Byte<(outs), OPC_LDX_ZPY>;
def LDX_ABS  : MVC6502Inst3Byte<(outs), OPC_LDX_ABS>;
def LDX_ABSY : MVC6502Inst3Byte<(outs), OPC_LDX_ABSY>;

def LDY_IMM  : MVC6502Inst2Byte<(outs), OPC_LDY_IMM>;
def LDY_ZP   : MVC6502Inst2Byte<(outs), OPC_LDY_ZP>;
def LDY_ZPX  : MVC6502Inst2Byte<(outs), OPC_LDY_ZPX>;
def LDY_ABS  : MVC6502Inst3Byte<(outs), OPC_LDY_ABS>;
def LDY_ABSX : MVC6502Inst3Byte<(outs), OPC_LDY_ABSX>;

def STX_ZP   : MVC6502Inst2Byte<(outs), OPC_STX_ZP>;
def STX_ZPY  : MVC6502Inst2Byte<(outs), OPC_STX_ZPY>;
def STX_ABS  : MVC6502Inst3Byte<(outs), OPC_STX_ABS>;

def STY_ZP   : MVC6502Inst2Byte<(outs), OPC_STY_ZP>;
def STY_ZPX  : MVC6502Inst2Byte<(outs), OPC_STY_ZPX>;
def STY_ABS  : MVC6502Inst3Byte<(outs), OPC_STY_ABS>;

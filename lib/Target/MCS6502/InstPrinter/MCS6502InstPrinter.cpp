//===- MCS6502InstPrinter.cpp - Convert MCS6502 MCInst to asm syntax ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Prints an MCS6502 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "MCS6502InstPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/NativeFormatting.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "MCS6502GenAsmWriter.inc"

void MCS6502InstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                   StringRef Annot,
                                   const MCSubtargetInfo &STI) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void MCS6502InstPrinter::printRegName(raw_ostream &O, unsigned RegNo) const {
  O << getRegisterName(RegNo);
}

void MCS6502InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isReg()) {
    printRegName(O, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI);
}

void MCS6502InstPrinter::printImmediate(const MCInst *MI, unsigned OpNo,
                                        raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &MO = MI->getOperand(OpNo);

  // TODO: Is it # or /?

  if (MO.isImm()) {
    O << "#";
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  O << "#";
  MO.getExpr()->print(O, &MAI, false);
}

void MCS6502InstPrinter::printAddress8(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCInstrDesc &Desc = MII.get(MI->getOpcode());
  assert((MI->getNumOperands() == 1) && "NumOperands is not 1");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI, false);
}

void MCS6502InstPrinter::printAddress8X(const MCInst *MI, unsigned OpNo,
                                        raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCInstrDesc &Desc = MII.get(MI->getOpcode());
  assert((MI->getNumOperands() == 1) && "NumOperands is not 1");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    O << ",X";
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI, false);
  O << ",X";
}

void MCS6502InstPrinter::printAddress8Y(const MCInst *MI, unsigned OpNo,
                                        raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCInstrDesc &Desc = MII.get(MI->getOpcode());
  assert((MI->getNumOperands() == 1) && "NumOperands is not 1");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    O << ",Y";
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI, false);
  O << ",Y";
}

void MCS6502InstPrinter::printAddress(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCInstrDesc &Desc = MII.get(MI->getOpcode());

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    if (Desc.isConditionalBranch()) {
      O << "#";
      llvm::write_integer(O, static_cast<int8_t>(MO.getImm()), 1,
                          IntegerStyle::Integer);
      return;
    }
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI, false);
}

void MCS6502InstPrinter::printAddressX(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    O << ",X";
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI, false);
  O << ",X";
}

void MCS6502InstPrinter::printAddressY(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    O << ",Y";
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI, false);
  O << ",Y";
}

void MCS6502InstPrinter::printIndAddress(const MCInst *MI, unsigned OpNo,
                                         raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    O << "(";
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    O << ")";
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  O << "(";
  MO.getExpr()->print(O, &MAI, false);
  O << ")";
}

void MCS6502InstPrinter::printIndXAddress(const MCInst *MI, unsigned OpNo,
                                          raw_ostream &O,
                                          const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    O << "(";
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    O << ",X)";
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  O << "(";
  MO.getExpr()->print(O, &MAI, false);
  O << ",X)";
}

void MCS6502InstPrinter::printIndYAddress(const MCInst *MI, unsigned OpNo,
                                          raw_ostream &O,
                                          const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");

  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    O << "(";
    llvm::write_hex(O, MO.getImm(), HexPrintStyle::PrefixLower);
    O << "),Y";
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  O << "(";
  MO.getExpr()->print(O, &MAI, false);
  O << "),Y";
}

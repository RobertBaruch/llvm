//===- MCS6502AsmParser.cpp - Parse MCS6502 assembly to MCInst instructions ==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MCS6502MCTargetDesc.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace {

struct MCS6502Operand;

class MCS6502AsmParser : public MCTargetAsmParser {
  // rename the misleading return values.
  constexpr static bool kSuccess{false};
  constexpr static bool kFailure{true};

  SMLoc getLoc() const { return getParser().getTok().getLoc(); }

  /// This happens first. `Name` contains the instruction mnemonic in lower
  /// case. This must parse all operands via getLexer() and getParser(),
  /// placing the operands into the Operands vector.
  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;

  /// After ParseInstruction() is done and succeeds, this happens. Based on
  /// the operands, set the Opcode.
  bool MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                               OperandVector &Operands, MCStreamer &Out,
                               uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;

  bool ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override {
    return kFailure; // never succeeds.
  }

  bool ParseDirective(AsmToken DirectiveID) override;

// Auto-generated instruction matching functions
#define GET_ASSEMBLER_HEADER
#include "MCS6502GenAsmMatcher.inc"

  // Do we need parseImmediage?
  OperandMatchResultTy parseImmediate(OperandVector &Operands);
  bool parseOperand(OperandVector &Operands);

public:
  enum MCS6502MatchResultTy {
    Match_Dummy = FIRST_TARGET_MATCH_RESULT_TY,
#define GET_OPERAND_DIAGNOSTIC_TYPES
#include "MCS6502GenAsmMatcher.inc"
#undef GET_OPERAND_DIAGNOSTIC_TYPES
  };

  MCS6502AsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser,
                   const MCInstrInfo &MII, const MCTargetOptions &Options)
      : MCTargetAsmParser(Options, STI, MII) {
    setAvailableFeatures(ComputeAvailableFeatures(STI.getFeatureBits()));
  }
};

/// MCS6502Operand - Instances of this class represent a parsed machine
/// instruction
struct MCS6502Operand : public MCParsedAsmOperand {
  enum KindTy {
    Token,
    Register,
    Immediate,
    SImm8,
  } Kind;

  struct RegOp {
    unsigned RegNum;
  };

  struct ImmOp {
    const MCExpr *Val;
  };

  SMLoc StartLoc, EndLoc;
  union {
    StringRef Tok;
    RegOp Reg;
    ImmOp Imm;
  };

  MCS6502Operand(KindTy K) : MCParsedAsmOperand(), Kind(K) {}

public:
  MCS6502Operand(const MCS6502Operand &o) : MCParsedAsmOperand() {
    Kind = o.Kind;
    StartLoc = o.StartLoc;
    EndLoc = o.EndLoc;
    switch (Kind) {
    case Register:
      Reg = o.Reg;
      break;
    case Immediate:
    case SImm8:
      Imm = o.Imm;
      break;
    case Token:
      Tok = o.Tok;
      break;
    }
  }

  bool isToken() const override { return Kind == Token; }
  bool isReg() const override { return Kind == Register; }
  bool isImm() const override { return Kind == Immediate; }
  bool isMem() const override { return false; }

  bool isImm8() const { return isConstantImm() && isUInt<8>(getConstantImm()); }
  bool isSImm8() const { return isConstantImm() && isInt<8>(getConstantImm()); }
  bool isImm16() const {
    // If the imm16 can fit in an imm8, then reject so that we can use the
    // zero-page version of the instruction.
    return !isImm8() && isConstantImm() && isUInt<16>(getConstantImm());
  }
  bool isAddr16() const {
    return isConstantImm() && isUInt<16>(getConstantImm());
  }

  bool isConstantImm() const {
    return isImm() && dyn_cast<MCConstantExpr>(getImm());
  }

  int64_t getConstantImm() const {
    const MCExpr *Val = getImm();
    return static_cast<const MCConstantExpr *>(Val)->getValue();
  }

  /// Gets location of the first token of this operand.
  SMLoc getStartLoc() const override { return StartLoc; }

  /// Gets location of the last token of this operand.
  SMLoc getEndLoc() const override { return EndLoc; }

  unsigned getReg() const override {
    assert(Kind == Register && "Invalid type access!");
    return Reg.RegNum;
  }

  const MCExpr *getImm() const {
    assert((Kind != Register && Kind != Token) && "Invalid type access!");
    return Imm.Val;
  }

  StringRef getToken() const {
    assert(Kind == Token && "Invalid type access!");
    return Tok;
  }

  void print(raw_ostream &OS) const override {
    switch (Kind) {
    case Immediate:
      OS << *getImm();
      break;
    case SImm8:
      OS << "<simm8 " << *getImm() << ">";
      break;
    case Register:
      OS << "<register " << getReg() << ">";
      break;
    case Token:
      OS << "'" << getToken() << "'";
      break;
    }
  }

  static std::unique_ptr<MCS6502Operand> createToken(StringRef Str, SMLoc S) {
    auto Op = make_unique<MCS6502Operand>(Token);
    Op->Tok = Str;
    Op->StartLoc = S;
    Op->EndLoc = S;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createReg(unsigned RegNo, SMLoc S,
                                                   SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Register);
    Op->Reg.RegNum = RegNo;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createImm(const MCExpr *Val, SMLoc S,
                                                   SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Immediate);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createSImm8(const MCExpr *Val, SMLoc S,
                                                     SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(SImm8);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  void addExpr(MCInst &Inst, const MCExpr *Expr) const {
    assert(Expr && "Expr shouldn't be null!");
    if (auto *CE = dyn_cast<MCConstantExpr>(Expr))
      Inst.addOperand(MCOperand::createImm(CE->getValue()));
    else
      Inst.addOperand(MCOperand::createExpr(Expr));
  }

  // Used by the TableGen code
  void addRegOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    Inst.addOperand(MCOperand::createReg(getReg()));
  }

  void addImmOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addSImm8Operands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }
};

} // namespace

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "MCS6502GenAsmMatcher.inc"

bool MCS6502AsmParser::ParseInstruction(ParseInstructionInfo &Info,
                                        StringRef Name, SMLoc NameLoc,
                                        OperandVector &Operands) {
  DEBUG_WITH_TYPE("asm-matcher", dbgs()
                                     << "ParseInstruction, " << Name << "\n");

  // First operand is token for instruction
  Operands.push_back(MCS6502Operand::createToken(Name, NameLoc));

  // Parse operands
  while (!getLexer().is(AsmToken::EndOfStatement)) {
    DEBUG_WITH_TYPE("asm-matcher",
                    dbgs() << "ParseInstruction, trying to parse an operand\n");
    if (parseOperand(Operands) == kFailure)
      return kFailure;
  }
  DEBUG_WITH_TYPE("asm-matcher",
                  dbgs() << "ParseInstruction, end of statement reached\n");
  getParser().Lex(); // Consume the EndOfStatement;
  return kSuccess;
}

bool MCS6502AsmParser::MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                                               OperandVector &Operands,
                                               MCStreamer &Out,
                                               uint64_t &ErrorInfo,
                                               bool MatchingInlineAsm) {
  MCInst Inst;
  SMLoc ErrorLoc;

  DEBUG_WITH_TYPE("asm-matcher", dbgs() << "MatchAndEmitInstruction\n");
  unsigned int result =
      MatchInstructionImpl(Operands, Inst, ErrorInfo, MatchingInlineAsm);
  switch (result) {
  default:
    return Error(IDLoc, "Unknown instruction format for mnemonic");
  case Match_Success:
    Inst.setLoc(IDLoc);
    Out.EmitInstruction(Inst, getSTI());
    return kSuccess;
  case Match_MissingFeature:
    return Error(IDLoc, "instruction use requires an option to be enabled");
  case Match_MnemonicFail:
    return Error(IDLoc, "unrecognized instruction mnemonic");
  case Match_InvalidOperand:
    ErrorLoc = IDLoc;
    if (ErrorInfo != ~0U) {
      if (ErrorInfo >= Operands.size())
        return Error(ErrorLoc, "too few operands for instruction");
      ErrorLoc = ((MCS6502Operand &)*Operands[ErrorInfo]).getStartLoc();
      if (ErrorLoc == SMLoc())
        ErrorLoc = IDLoc;
    }
    return Error(ErrorLoc, "invalid operand for instruction");
  }

  llvm_unreachable("Unreachable");
}

OperandMatchResultTy MCS6502AsmParser::parseImmediate(OperandVector &Operands) {
  if (getLexer().isNot(AsmToken::Hash)) {
    Error(getLoc(), "Expected '#'");
    return MatchOperand_ParseFail;
  }
  getParser().Lex(); // Eat '#'

  switch (getLexer().getKind()) {
  default:
    return MatchOperand_ParseFail;
  case AsmToken::Minus:
  case AsmToken::Plus:
  case AsmToken::Integer:
  case AsmToken::String:
    break;
  }

  const MCExpr *IdVal;
  SMLoc S = getLoc();
  if (getParser().parseExpression(IdVal))
    return MatchOperand_ParseFail;

  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  Operands.push_back(MCS6502Operand::createImm(IdVal, S, E));
  return MatchOperand_Success;
}

// The possible operand patterns are:
// #imm8
// addr8
// addr8,X
// addr8,Y
// addr16
// addr16,X
// addr16,Y
// (addr16,X)
// (addr16),Y
// (addr16)
//
// We don't allow expressions for the moment, only integers.
bool MCS6502AsmParser::parseOperand(OperandVector &Operands) {
  DEBUG_WITH_TYPE("asm-matcher", dbgs() << "parseOperand, starts with '"
                                        << getLexer().getTok().getString()
                                        << "'\n");
  switch (getLexer().getKind()) {
  default:
    return kFailure;

  // We don't include AsmToken::Comma as an operand because that is apparently
  // ignored when doing the second pass match against instructions.
  case AsmToken::LParen:
  case AsmToken::RParen:
  case AsmToken::Hash:
    Operands.push_back(
        MCS6502Operand::createToken(getLexer().getTok().getString(), getLoc()));
    getParser().Lex(); // Eat token
    return kSuccess;

  case AsmToken::Comma:
    getParser().Lex(); // Eat ','
    if (getLexer().getKind() != AsmToken::Identifier)
      return kFailure;
    if (!getLexer().getTok().getString().equals("X") &&
        !getLexer().getTok().getString().equals("Y"))
      return kFailure;
    Operands.push_back(
        MCS6502Operand::createToken(getLexer().getTok().getString(), getLoc()));
    getParser().Lex(); // Eat token
    return kSuccess;

  case AsmToken::Plus:
  case AsmToken::Minus:
  case AsmToken::Integer:
    const MCExpr *IdVal;
    SMLoc S = getLoc();
    if (getParser().parseExpression(IdVal))
      return kFailure;
    SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createImm(IdVal, S, E));
    return kSuccess;
  }

  Error(getLoc(), "unknown operand");
  return kFailure;
}

bool MCS6502AsmParser::ParseDirective(AsmToken DirectiveID) { return kFailure; }

extern "C" void LLVMInitializeMCS6502AsmParser() {
  RegisterMCAsmParser<MCS6502AsmParser> X(getTheMCS6502Target());
}

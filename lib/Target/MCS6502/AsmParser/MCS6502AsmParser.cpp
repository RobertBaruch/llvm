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

#define DEBUG_TYPE "asmparser"

using namespace llvm;

namespace {

struct MCS6502Operand;

class MCS6502AsmParser : public MCTargetAsmParser {
  // rename the misleading return values.
  constexpr static bool kSuccess{false};
  constexpr static bool kFailure{true};

  SMLoc getLoc() const { return getParser().getTok().getLoc(); }

  /// Called by AsmParser::parseStatement, after having parsed a mnemonic.
  /// The mnemonic is the identifier found after zero or more labels that is
  /// also not one of the pre-defined directives. Directives start with a dot.
  ///
  /// `Name` contains the instruction mnemonic in lower case. This function must
  /// first place the `Name` and `NameLoc` in the Operands vector as a Token.
  /// Then the function must parse all operands via getLexer() and getParser(),
  /// placing the operands into the Operands vector.
  ///
  /// AsmParser looked for an identifier as the mnemonic, and an identifier, to
  /// the AsmParser, is the following regex:
  ///   [$@][a-zA-Z_.][a-zA-Z0-9_$.@]*
  ///
  /// After this, AsmParser::parseStatement will call MatchAndEmitInstruction
  /// with the Operands found.
  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;

  /// After ParseInstruction() is done and succeeds, this is called by
  /// AsmParser::parseStatement. Typically this will first call the generated
  /// MatchInstructionImpl in <Target>GenAsmMatcher.inc. That function goes
  /// through all Instruction structures from the table that have the same
  /// first word in the AsmString field. (TODO: confirm this)
  ///
  /// For each such Instruction found, MatchInstructionImpl goes through each
  /// operand in the Operands vector and matches it against the operand classes
  /// found in the rest of the AsmString field.
  ///
  /// This is done by first calling validateOperandClass. If the operand is
  /// a Token, then we just check if the pseudo-class represented by the token
  /// is a subclass of the required class. A psuedo-class is created for each
  /// unique token in an AsmString.
  ///
  /// Otherwise, if the operand is not a Token, we call the Operand's
  /// is<class> method to see if it matches.
  ///
  /// If all operands match, then that Instruction is chosen, and the MCInst
  /// is filled. At that point this function should do the following:
  ///
  ///      Inst.setLoc(IDLoc);
  ///      Opcode = Inst.getOpcode();
  ///      Out.EmitInstruction(Inst, getSTI());
  ///      return kSuccess;
  ///
  /// Otherwise, on failure, this function should return an Error with
  /// some appropriate message.
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

  /// Parses:
  /// [#/]<expression>
  bool parseImmediate(OperandVector &Operands);

  /// Parses:
  /// (expr)
  /// (expr,X)
  /// (expr),Y
  bool parseIndirect(OperandVector &Operands);

  /// Parses:
  /// expr
  /// expr,X
  /// expr,Y
  bool parseAbsolute(OperandVector &Operands);

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
    IndXAddr,
    IndYAddr,
    IndAddr,
    AddrX,
    AddrY,
    Addr,
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
    case IndAddr:
    case IndXAddr:
    case IndYAddr:
    case Addr:
    case AddrX:
    case AddrY:
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
  bool isAddr() const { return Kind == Addr; }
  bool isAddrX() const { return Kind == AddrX; }
  bool isAddrY() const { return Kind == AddrY; }
  bool isIndAddr() const { return Kind == IndAddr; }
  bool isIndXAddr() const { return Kind == IndXAddr; }
  bool isIndYAddr() const { return Kind == IndYAddr; }
  bool isImmediate() const { return Kind == Immediate; }

  bool isImm8() const { return isConstantImm() && isUInt<8>(getConstantImm()); }
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
    case Addr:
      OS << "<addr " << *getImm() << ">";
      break;
    case AddrX:
      OS << "<addr,x " << *getImm() << ">";
      break;
    case AddrY:
      OS << "<addr,y " << *getImm() << ">";
      break;
    case IndAddr:
      OS << "<(addr) " << *getImm() << ">";
      break;
    case IndXAddr:
      OS << "<(addr,x) " << *getImm() << ">";
      break;
    case IndYAddr:
      OS << "<(addr),y " << *getImm() << ">";
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

  static std::unique_ptr<MCS6502Operand> createIndAddr(const MCExpr *Val,
                                                       SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(IndAddr);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createIndXAddr(const MCExpr *Val,
                                                        SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(IndXAddr);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createIndYAddr(const MCExpr *Val,
                                                        SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(IndYAddr);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createAddr(const MCExpr *Val, SMLoc S,
                                                    SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Addr);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createAddrX(const MCExpr *Val, SMLoc S,
                                                     SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(AddrX);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createAddrY(const MCExpr *Val, SMLoc S,
                                                     SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(AddrY);
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
};

} // namespace

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "MCS6502GenAsmMatcher.inc"

bool MCS6502AsmParser::ParseInstruction(ParseInstructionInfo &Info,
                                        StringRef Name, SMLoc NameLoc,
                                        OperandVector &Operands) {
  LLVM_DEBUG(dbgs() << "ParseInstruction, " << Name << "\n");

  // First operand is token for instruction
  Operands.push_back(MCS6502Operand::createToken(Name, NameLoc));

  // Parse operands
  while (!getLexer().is(AsmToken::EndOfStatement)) {
    LLVM_DEBUG(dbgs() << "ParseInstruction, trying to parse an operand\n");
    if (parseOperand(Operands) == kFailure)
      return kFailure;
  }
  LLVM_DEBUG(dbgs() << "ParseInstruction, end of statement reached\n");
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

  LLVM_DEBUG(dbgs() << "MatchAndEmitInstruction\n");
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

// [#/]<expression>
bool MCS6502AsmParser::parseImmediate(OperandVector &Operands) {
  SMLoc S = getLoc();

  if (getLexer().isNot(AsmToken::Hash) && getLexer().isNot(AsmToken::Slash)) {
    Error(getLoc(), "Expected '#' or '/'");
    return kFailure;
  }
  bool UseHighByte = getLexer().getKind() == AsmToken::Slash;
  getParser().Lex(); // Eat '#' or '/'.

  const MCExpr *IdVal;
  if (getParser().parseExpression(IdVal) == kFailure)
    return kFailure;

  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  Operands.push_back(MCS6502Operand::createImm(IdVal, S, E));
  return kSuccess;
}

// <expression>
// <expression>,X
// <expression>,Y
bool MCS6502AsmParser::parseAbsolute(OperandVector &Operands) {
  SMLoc S = getLoc();

  const MCExpr *IdVal;
  LLVM_DEBUG(dbgs() << "  parseOperand: absolute, " << getLexer().getKind()
                    << "\n");
  if (getParser().parseExpression(IdVal) == kFailure)
    return kFailure;

  if (getLexer().isNot(AsmToken::Comma)) {
    SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createAddr(IdVal, S, E));
    return kSuccess;
  }
  getParser().Lex(); // Eat ','.

  StringRef reg;
  if (getParser().parseIdentifier(reg) == kFailure)
    return kFailure;

  if (!reg.equals_lower("x") && !reg.equals_lower("y"))
    return kFailure;

  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  Operands.push_back(reg.equals_lower("x")
                         ? MCS6502Operand::createAddrX(IdVal, S, E)
                         : MCS6502Operand::createAddrY(IdVal, S, E));
  return kSuccess;
}

// (<expression>)
// (<expression>,X)
// (<expression>),Y
bool MCS6502AsmParser::parseIndirect(OperandVector &Operands) {
  SMLoc S = getLoc();

  if (getLexer().isNot(AsmToken::LParen)) {
    Error(getLoc(), "Expected '('");
    return kFailure;
  }
  getParser().Lex(); // Eat '('.

  const MCExpr *IdVal;
  if (getParser().parseExpression(IdVal) == kFailure) {
    LLVM_DEBUG(dbgs() << "  parseOperand: indirect, failed expr\n");
    return kFailure;
  }

  LLVM_DEBUG(dbgs() << "  parseOperand: indirect, next after expr: "
                    << getLexer().getKind() << "\n");
  switch (getLexer().getKind()) {
  default:
    return kFailure;

  case AsmToken::Comma: {
    getParser().Lex(); // Eat ','.
    LLVM_DEBUG(dbgs() << "  parseOperand: (indirect,)\n");
    if (getLexer().isNot(AsmToken::Identifier))
      return kFailure;
    StringRef x;
    if (getParser().parseIdentifier(x) == kFailure || !x.equals_lower("x"))
      return kFailure;
    if (getLexer().isNot(AsmToken::RParen))
      return kFailure;
    getParser().Lex(); // Eat ')'.
    SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createIndXAddr(IdVal, S, E));
    return kSuccess;
  }

  case AsmToken::RParen: {
    getParser().Lex(); // Eat ')'.
    if (getLexer().is(AsmToken::Comma)) {
      getParser().Lex(); // Eat ','.
      LLVM_DEBUG(dbgs() << "  parseOperand: (indirect),\n");
      StringRef y;
      if (getParser().parseIdentifier(y) == kFailure || !y.equals_lower("y"))
        return kFailure;
      SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
      Operands.push_back(MCS6502Operand::createIndYAddr(IdVal, S, E));
      return kSuccess;
    }
    LLVM_DEBUG(dbgs() << "  parseOperand: (indirect)\n");
    SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createIndAddr(IdVal, S, E));
    return kSuccess;
  }
  }
}

bool MCS6502AsmParser::parseOperand(OperandVector &Operands) {
  LLVM_DEBUG(dbgs() << "parseOperand, starts with '"
                    << getLexer().getTok().getString() << "'\n");
  switch (getLexer().getKind()) {
  case AsmToken::Slash:
  case AsmToken::Hash:
    LLVM_DEBUG(dbgs() << "parseOperand: slash/hash\n");
    return parseImmediate(Operands);

  case AsmToken::LParen:
    LLVM_DEBUG(dbgs() << "parseOperand: lparen\n");
    return parseIndirect(Operands);

  case AsmToken::EndOfStatement:
    return kSuccess;

  default:
    LLVM_DEBUG(dbgs() << "parseOperand: absolute\n");
    return parseAbsolute(Operands);
  }
}

bool MCS6502AsmParser::ParseDirective(AsmToken DirectiveID) { return kFailure; }

extern "C" void LLVMInitializeMCS6502AsmParser() {
  RegisterMCAsmParser<MCS6502AsmParser> X(getTheMCS6502Target());
}

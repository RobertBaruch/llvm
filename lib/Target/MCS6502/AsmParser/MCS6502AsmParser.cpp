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

class MCS6502Operand;

class MCS6502AsmParser : public MCTargetAsmParser {
  static constexpr bool kParseSuccess = false;
  static constexpr bool kParseFailure = true;

  SMLoc getLoc() const { return getParser().getTok().getLoc(); }

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

  bool ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;

  /// Called by AsmParser::parseStatement, after having parsed a mnemonic.
  /// The mnemonic is the identifier found after zero or more labels that is
  /// also not one of the pre-defined directives. Directives start with a
  /// dot.
  ///
  /// `Name` contains the instruction mnemonic in lower case. This function
  /// must first place the `Name` and `NameLoc` in the Operands vector as a
  /// Token. Then the function must parse all operands via getLexer() and
  /// getParser(), placing the operands into the Operands vector.
  ///
  /// AsmParser looked for an identifier as the mnemonic, and an identifier,
  /// to the AsmParser, is the following regex:
  ///   [$@][a-zA-Z_.][a-zA-Z0-9_$.@]*
  ///
  /// After this, AsmParser::parseStatement will call
  /// MatchAndEmitInstruction with the Operands found.
  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;

  bool ParseDirective(AsmToken DirectiveID) override;

// Auto-generated instruction matching functions
#define GET_ASSEMBLER_HEADER
#include "MCS6502GenAsmMatcher.inc"

  bool parseIndirect(OperandVector &Operands);
  bool parseLowImmediate(OperandVector &Operands);
  bool parseHighImmediate(OperandVector &Operands);

  OperandMatchResultTy parseImmediate(OperandVector &Operands);
  bool parseRegister(unsigned &RegNo);

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
class MCS6502Operand : public MCParsedAsmOperand {
public:
  enum KindTy {
    Token,
    Register,
    Immediate,
    Indirect,
    PreIndirect,
    PostIndirect,
    ZP,
    Abs,
    AbsX,
    AbsY,
    ZPX,
    ZPY,
    Rel,
  } Kind;

  const MCExpr *Val;
  unsigned RegNum;

  SMLoc StartLoc, EndLoc;
  std::string Tok;

  MCS6502Operand(KindTy K) : MCParsedAsmOperand(), Kind(K) {}
  MCS6502Operand(const MCS6502Operand &o) = default;

  bool isToken() const override { return Kind == Token; }
  bool isReg() const override { return Kind == Register; }
  bool isImm() const override { return Kind == Immediate; }
  bool isMem() const override { return false; }

  bool isInd() const { return Kind == Indirect; }
  bool isPreInd() const { return Kind == PreIndirect; }
  bool isPostInd() const { return Kind == PostIndirect; }
  bool isZP() const { return Kind == ZP; }
  bool isAbs() const { return Kind == Abs; }
  bool isAbsX() const { return Kind == AbsX; }
  bool isAbsY() const { return Kind == AbsY; }
  bool isZPX() const { return Kind == ZPX; }
  bool isZPY() const { return Kind == ZPY; }
  bool isRel() const { return Kind == Rel || isZP() || isAbs(); }

  bool isAnyInd() const { return isInd() || isPreInd() || isPostInd(); }

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
    return RegNum;
  }

  const MCExpr *getImm() const {
    assert(Kind == Immediate && "Invalid type access!");
    return Val;
  }

  const MCExpr *getZPImm() const {
    assert(Kind == ZP && "Invalid type access!");
    return Val;
  }

  const MCExpr *getAbsImm() const {
    assert(Kind == Abs && "Invalid type access!");
    return Val;
  }

  const MCExpr *getAbsXImm() const {
    assert(Kind == AbsX && "Invalid type access!");
    return Val;
  }

  const MCExpr *getAbsYImm() const {
    assert(Kind == AbsY && "Invalid type access!");
    return Val;
  }

  const MCExpr *getZPXImm() const {
    assert(Kind == ZPX && "Invalid type access!");
    return Val;
  }

  const MCExpr *getZPYImm() const {
    assert(Kind == ZPY && "Invalid type access!");
    return Val;
  }

  const MCExpr *getRelImm() const {
    assert(isRel() && "Invalid type access!");
    return Val;
  }

  StringRef getToken() const {
    assert(Kind == Token && "Invalid type access!");
    return Tok;
  }

  unsigned getIndReg() const {
    assert((isPreInd() || isPostInd()) && "Invalid type access!");
    return RegNum;
  }

  const MCExpr *getIndImm() const {
    assert(isAnyInd() && "Invalid type access!");
    return Val;
  }

  void print(raw_ostream &OS) const override {
    switch (Kind) {
    case Immediate:
      OS << "<immediate " << *getImm() << ">";
      break;
    case Rel:
      OS << "<rel " << *getRelImm() << ">";
      break;
    case ZP:
      OS << "<zp " << *getZPImm() << ">";
      break;
    case ZPX:
      OS << "<zpx " << *getZPXImm() << ", X>";
      break;
    case ZPY:
      OS << "<zpy " << *getZPYImm() << ", Y>";
      break;
    case Abs:
      OS << "<absolute " << *getAbsImm() << ">";
      break;
    case AbsX:
      OS << "<absolutex " << *getAbsXImm() << ", X>";
      break;
    case AbsY:
      OS << "<absolutey " << *getAbsYImm() << ", Y>";
      break;
    case Register:
      OS << "<register " << getReg() << ">";
      break;
    case Indirect:
      OS << "<indirect (" << *getIndImm() << ")>";
      break;
    case PreIndirect:
      OS << "<preindirect (" << *getIndImm() << ", register " << getIndReg()
         << ")>";
      break;
    case PostIndirect:
      OS << "<postindirect (" << *getIndImm() << "), register " << getIndReg()
         << ">";
      break;
    case Token:
      OS << "'" << getToken() << "'";
      break;
    }
  }

  static std::unique_ptr<MCS6502Operand> createToken(std::string Str, SMLoc S) {
    auto Op = make_unique<MCS6502Operand>(Token);
    Op->Tok = Str;
    Op->StartLoc = S;
    Op->EndLoc = S;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createReg(unsigned RegNo, SMLoc S,
                                                   SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Register);
    Op->RegNum = RegNo;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createImm(const MCExpr *Val, SMLoc S,
                                                   SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Immediate);
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createRel(const MCExpr *Val, SMLoc S,
                                                   SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Rel);
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    LLVM_DEBUG(dbgs() << "Adding relative operand");
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createZP(const MCExpr *Val, SMLoc S,
                                                  SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(ZP);
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createAbs(const MCExpr *Val, SMLoc S,
                                                   SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Abs);
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createAbsX(const MCExpr *Val, unsigned RegNo, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(AbsX);
    Op->RegNum = RegNo;
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createAbsY(const MCExpr *Val, unsigned RegNo, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(AbsY);
    Op->RegNum = RegNo;
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createZPX(const MCExpr *Val, unsigned RegNo, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(ZPX);
    Op->RegNum = RegNo;
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createZPY(const MCExpr *Val, unsigned RegNo, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(ZPY);
    Op->RegNum = RegNo;
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createLowImm(const MCExpr *Val, MCContext &Ctx, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Immediate);
    Op->Val = Val;
    // MCBinaryExpr::createAnd(Val, MCConstantExpr::create(0xFF, Ctx), Ctx);
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createHighImm(const MCExpr *Val, MCContext &Ctx, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Immediate);
    Op->Val = Val;
    // Op->Val =
    //     MCBinaryExpr::createLShr(Val, MCConstantExpr::create(8, Ctx), Ctx);
    // Op->Val = MCBinaryExpr::createAnd(Op->Val,
    //                                   MCConstantExpr::create(0xFF, Ctx),
    //                                   Ctx);
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand> createInd(const MCExpr *Val, SMLoc S,
                                                   SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(Indirect);
    Op->RegNum = 0;
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createPreInd(const MCExpr *Val, unsigned RegNo, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(PreIndirect);
    Op->RegNum = RegNo;
    Op->Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<MCS6502Operand>
  createPostInd(const MCExpr *Val, unsigned RegNo, SMLoc S, SMLoc E) {
    auto Op = make_unique<MCS6502Operand>(PostIndirect);
    Op->RegNum = RegNo;
    Op->Val = Val;
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

  void addRelOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getRelImm());
  }

  void addPreIndOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getIndImm());
  }

  void addPostIndOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getIndImm());
  }

  void addIndOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getIndImm());
  }

  void addZPOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getZPImm());
  }

  void addAbsOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getAbsImm());
  }

  void addAbsXOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getAbsXImm());
  }
  void addAbsYOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getAbsYImm());
  }
  void addZPXOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getZPXImm());
  }
  void addZPYOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getZPYImm());
  }
};

} // namespace

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "MCS6502GenAsmMatcher.inc"

bool MCS6502AsmParser::MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                                               OperandVector &Operands,
                                               MCStreamer &Out,
                                               uint64_t &ErrorInfo,
                                               bool MatchingInlineAsm) {
  MCInst Inst;
  SMLoc ErrorLoc;

  switch (MatchInstructionImpl(Operands, Inst, ErrorInfo, MatchingInlineAsm)) {
  default:
    break;
  case Match_Success:
    Inst.setLoc(IDLoc);
    Out.EmitInstruction(Inst, getSTI());
    return false;
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

  llvm_unreachable("Unknown match type detected");
}

bool MCS6502AsmParser::ParseRegister(unsigned &RegNo, SMLoc &StartLoc,
                                     SMLoc &EndLoc) {
  const AsmToken &Tok = getParser().getTok();
  StartLoc = Tok.getLoc();
  EndLoc = Tok.getEndLoc();
  RegNo = 0;
  StringRef Name = getLexer().getTok().getIdentifier();

  if (!MatchRegisterName(Name)) {
    getParser().Lex(); // Eat identifier token.
    return kParseSuccess;
  }

  return Error(StartLoc, "invalid register name");
}

bool MCS6502AsmParser::parseRegister(unsigned &RegNo) {
  switch (getLexer().getKind()) {
  default:
    return kParseFailure;
  case AsmToken::Identifier:
    StringRef Name = getLexer().getTok().getIdentifier();
    RegNo = MatchRegisterName(Name);
    if (RegNo == 0)
      return kParseFailure;
    LLVM_DEBUG(dbgs() << "  parseRegister: " << RegNo << "\n");
    getLexer().Lex(); // Eat register.
  }
  return kParseSuccess;
}

OperandMatchResultTy MCS6502AsmParser::parseImmediate(OperandVector &Operands) {
  switch (getLexer().getKind()) {
  default:
    return MatchOperand_NoMatch;
  case AsmToken::LParen:
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

bool MCS6502AsmParser::parseLowImmediate(OperandVector &Operands) {
  const MCExpr *IdVal;
  SMLoc S = getLoc();

  Operands.push_back(MCS6502Operand::createToken("#", S));

  if (getParser().parseExpression(IdVal))
    return Error(getLoc(), "Failed to parse expression");

  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  Operands.push_back(MCS6502Operand::createLowImm(IdVal, getContext(), S, E));
  return kParseSuccess;
}

bool MCS6502AsmParser::parseHighImmediate(OperandVector &Operands) {
  const MCExpr *IdVal;
  SMLoc S = getLoc();

  Operands.push_back(MCS6502Operand::createToken("/", S));

  if (getParser().parseExpression(IdVal))
    return Error(getLoc(), "Failed to parse expression");

  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  Operands.push_back(MCS6502Operand::createHighImm(IdVal, getContext(), S, E));
  return kParseSuccess;
}

bool MCS6502AsmParser::parseIndirect(OperandVector &Operands) {
  SMLoc S = getLoc();
  const MCExpr *IdVal;
  unsigned RegNo = 0;

  Operands.push_back(MCS6502Operand::createToken("(", S));

  if (getParser().parseExpression(IdVal) == kParseFailure) {
    LLVM_DEBUG(dbgs() << "  parseOperand: indirect, failed expr\n");
    return Error(getLoc(), "Failed to parse expression");
  }

  LLVM_DEBUG(dbgs() << "  parseOperand: indirect, next after expr: "
                    << getLexer().getKind() << "\n");
  switch (getLexer().getKind()) {
  default:
    return Error(getLoc(), "Expected ',' or ')' after expression");

  case AsmToken::Comma: {
    SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createPreInd(IdVal, RegNo, S, E));

    getParser().Lex(); // Eat ','.
    LLVM_DEBUG(dbgs() << "  parseOperand: (indirect,)\n");
    S = getLoc();
    if (parseRegister(RegNo) == kParseFailure)
      return kParseFailure;
    E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createReg(RegNo, S, E));
    if (getLexer().isNot(AsmToken::RParen))
      return Error(getLoc(), "Expected ')' after indirect value");
    getParser().Lex(); // Eat ')'.

    Operands.push_back(MCS6502Operand::createToken(")", E));
    return kParseSuccess;
  }

  case AsmToken::RParen: {
    getParser().Lex(); // Eat ')'.
    if (getLexer().is(AsmToken::Comma)) {
      getParser().Lex(); // Eat ','.
      LLVM_DEBUG(dbgs() << "  parseOperand: (indirect),\n");
      if (parseRegister(RegNo) == kParseFailure)
        return kParseFailure;
      SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
      Operands.push_back(MCS6502Operand::createPostInd(IdVal, RegNo, S, E));
      Operands.push_back(MCS6502Operand::createToken(")", S));
      Operands.push_back(MCS6502Operand::createReg(RegNo, S, E));
      return kParseSuccess;
    }
    LLVM_DEBUG(dbgs() << "  parseOperand: (indirect)\n");
    SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createInd(IdVal, S, E));
    Operands.push_back(MCS6502Operand::createToken(")", S));
    return kParseSuccess;
  }
  }
}

bool MCS6502AsmParser::ParseInstruction(ParseInstructionInfo &Info,
                                        StringRef Name, SMLoc NameLoc,
                                        OperandVector &Operands) {
  SMLoc S = getLoc();
  const MCExpr *IdVal;
  unsigned RegNo = 0;

  // First operand is token for instruction
  Operands.push_back(MCS6502Operand::createToken(Name.upper(), NameLoc));

  // If there are no operands, then finish
  if (getLexer().is(AsmToken::EndOfStatement))
    return kParseSuccess;

  if (getLexer().is(AsmToken::Hash)) {
    getParser().Lex(); // Eat '#'
    if (parseLowImmediate(Operands) == kParseFailure)
      return kParseFailure;
  }

  else if (getLexer().is(AsmToken::Slash)) {
    getParser().Lex(); // Eat '/'
    if (parseHighImmediate(Operands) == kParseFailure)
      return kParseFailure;
  }

  else if (getLexer().is(AsmToken::LParen)) {
    getParser().Lex(); // Eat '('
    if (parseIndirect(Operands) == kParseFailure)
      return kParseFailure;
  }

  else if (parseRegister(RegNo) == kParseSuccess) {
    SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
    Operands.push_back(MCS6502Operand::createReg(RegNo, S, E));
  }

  else if (getParser().parseExpression(IdVal) == kParseSuccess) {
    SMLoc SAddr = S;
    SMLoc EAddr = SMLoc::getFromPointer(S.getPointer() - 1);
    bool zp = false;

    if (dyn_cast<MCConstantExpr>(IdVal)) {
      int64_t sval = static_cast<const MCConstantExpr *>(IdVal)->getValue();
      uint64_t val = static_cast<uint64_t>(sval);
      if (val <= 0xFF) {
        zp = true;
      } else if (val <= 0xFFFF) {
        zp = false;
      } else if (sval >= -0x80 && sval < 0) {
        zp = true;
      } else {
        return Error(S, "value must be 16 bits or less");
      }
    } else {
      zp = false;
    }

    SMLoc E = getLoc();
    if (getLexer().is(AsmToken::Comma)) {
      getParser().Lex(); // Eat ','
      S = getLoc();
      if (parseRegister(RegNo) == kParseFailure)
        return kParseFailure;
      E = SMLoc::getFromPointer(S.getPointer() - 1);
    }

    if (RegNo == 0) {
      Operands.push_back(zp ? MCS6502Operand::createZP(IdVal, SAddr, EAddr)
                            : MCS6502Operand::createAbs(IdVal, SAddr, EAddr));
    } else {
      Operands.push_back(
          RegNo == MCS6502::X
              ? (zp ? MCS6502Operand::createZPX(IdVal, RegNo, S, E)
                    : MCS6502Operand::createAbsX(IdVal, RegNo, S, E))
              : (zp ? MCS6502Operand::createZPY(IdVal, RegNo, S, E)
                    : MCS6502Operand::createAbsY(IdVal, RegNo, S, E)));
      Operands.push_back(MCS6502Operand::createReg(RegNo, S, E));
    }
  }

  if (getLexer().isNot(AsmToken::EndOfStatement)) {
    SMLoc Loc = getLexer().getLoc();
    getParser().eatToEndOfStatement();
    return Error(Loc, "unexpected token");
  }

  getParser().Lex(); // Consume the EndOfStatement;
  return kParseSuccess;
}

bool MCS6502AsmParser::ParseDirective(AsmToken DirectiveID) { return true; }

extern "C" void LLVMInitializeMCS6502AsmParser() {
  RegisterMCAsmParser<MCS6502AsmParser> X(getTheMCS6502Target());
}

//===-- MCS6502AsmPrinter.cpp - MCS6502 assembly writer -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the MCS6502 assembly language.
//
//===----------------------------------------------------------------------===//

#include "MCS6502.h"
#include "MCS6502TargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {

class MCS6502AsmPrinter : public AsmPrinter {
public:
  explicit MCS6502AsmPrinter(TargetMachine &TM,
                             std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "MCS6502 Assembly Printer"; }

  void EmitInstruction(const MachineInstr *MI) override;

  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);
};

// Simple psuedo-instructions have their lowering (with expansion to real
// instructions) auto-generated.
#include "MCS6502GenMCPseudoLowering.inc"

void MCS6502AsmPrinter::EmitInstruction(const MachineInstr *MI) {
  // Do any auto-generated pseudo-lowerings.
  if (emitPseudoExpansionLowering(*OutStreamer, MI))
    return;

  MCInst TmpInst;
  LowerMCS6502MachineInstrToMCInst(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization
extern "C" void LLVMInitializeMCS6502AsmPrinter() {
  RegisterAsmPrinter<MCS6502AsmPrinter> X(getTheMCS6502Target());
}

} // namespace

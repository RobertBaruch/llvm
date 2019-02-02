//=== MCS6502ISelLowering.h - Dag lowering interface for MCS6502 -*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that MCS6502 uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MCS6502_MCS6502ISELLOWERING_H
#define LLVM_LIB_TARGET_MCS6502_MCS6502ISELLOWERING_H

#include "MCS6502.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class MCS6502Subtarget;

namespace MCS6502ISD {

enum NodeType : unsigned { FIRST_NUMBER = ISD::BUILTIN_OP_END, RET_FLAG };

} // namespace MCS6502ISD

class MCS6502TargetLowering : public TargetLowering {
  const MCS6502Subtarget &Subtarget;

public:
  explicit MCS6502TargetLowering(const TargetMachine &TM,
                                 const MCS6502Subtarget &STI);

  // Provide custom lowering hooks for some operations.
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  // Returns the name of a target specific DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

private:
  // Lower incoming arguments, copy physregs into vregs
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

  bool shouldConvertConstantLoadToIntImm(const APInt &Imm,
                                         Type *Ty) const override {
    return true;
  }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MCS6502_MCS6502ISELLOWERING_H

//===-- MCS6502InstrInfo.td - MCS6502 Instructions ---------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

def ADC_INDX : Instruction {
  let Namespace = "MCS6502";

  bits<24> Inst;
  bits<16> addr;

  let Size = 3;
  let Inst{23-16} = 0x61;
  let Inst{15-8} = addr{7-0};
  let Inst{7-0} = addr{15-8};
  let AsmString = "ADC ($addr,X)";
  let OutOperandList = (outs);
  let InOperandList = (ins i16imm:$addr);
}

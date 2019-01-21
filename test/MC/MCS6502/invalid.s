; RUN: not llvm-mc -triple mcs6502 < %s 2>&1 | FileCheck %s

; Out of range immediate
lda #0xF000 ; CHECK: [[@LINE]]:1: error: Unknown instruction format for mnemonic

; invalid mnemonic
tas ; CHECK: [[@LINE]]:1: error: unrecognized instruction mnemonic

; Too few operands
lda ; CHECK: [[@LINE]]:1: error: too few operands for instruction

; Too many operands
lda 0xFF, 0xFF
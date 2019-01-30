; RUN: not llvm-mc -triple mcs6502 < %s 2>&1 | FileCheck %s

; Out of range immediate
lda #0xF000 ; CHECK: [[@LINE]]:1: error: Value after '#' too large

; Address too high
lda (0x6677,X) ; CHECK: [[@LINE]]:1: error: Value of indirect address too large

; Address too high
lda (0x6677),Y ; CHECK: [[@LINE]]:1: error: Value of indirect address too large

; invalid mnemonic
tas ; CHECK: [[@LINE]]:1: error: unrecognized instruction mnemonic

; Too few operands
lda ; CHECK: [[@LINE]]:1: error: too few operands for instruction

; Too many operands
lda 0xFF, 0xFF ; CHECK: [[@LINE]]:1: error: Expected 'X' or 'Y' after '<expression>,'

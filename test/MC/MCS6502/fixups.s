; RUN: llvm-mc %s -triple=mcs6502 -show-encoding \
; RUN:     | FileCheck -check-prefixes=CHECK-FIXUP %s
; RUN: llvm-mc %s -triple=mcs6502 -filetype=obj \
; RUN:     | llvm-objdump -d - | FileCheck -check-prefixes=CHECK-INST %s
; RUN: llvm-mc %s -triple=mcs6502 -filetype=obj \
; RUN:     | llvm-readobj -r - | FileCheck -check-prefixes=CHECK-REL %s

lda val16
; CHECK-FIXUP: fixup A - offset: 0, value: val16, kind: fixup_mcs6502_symbol16
sta val8
; CHECK-FIXUP: fixup A - offset: 0, value: val8, kind: fixup_mcs6502_symbol16
ldx #val16
; CHECK-FIXUP: fixup A - offset: 0, value: val16, kind: fixup_mcs6502_symbol8
ldy #val8
; CHECK-FIXUP: fixup A - offset: 0, value: val8, kind: fixup_mcs6502_symbol8

.LBB0:
tay

bcc .LBB0
; CHECK-INST: BCC 0xff
; CHECK-FIXUP: fixup A - offset: 0, value: .LBB0, kind: fixup_mcs6502_branch

bcs .LBB1
; CHECK-INST: BCS 0x12
; CHECK-FIXUP: fixup A - offset: 0, value: .LBB1, kind: fixup_mcs6502_branch

.fill 16
.LBB1:
tay

jmp .LBB1
; CHECK-INST: JMP 0x0
; CHECK-FIXUP: fixup A - offset: 0, value: .LBB1, kind: fixup_mcs6502_symbol16
; CHECK-REL: 0x20 R_MCS6502_SYMBOL16 .text 0x1F
jsr .LBB0
; CHECK-INST: JSR 0x0
; CHECK-FIXUP: fixup A - offset: 0, value: .LBB0, kind: fixup_mcs6502_symbol16
; CHECK-REL: 0x23 R_MCS6502_SYMBOL16 .text 0xA

.set val16, 0x1234
.set val8, 0xAB

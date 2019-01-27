; RUN: llvm-mc -triple=mcs6502 -show-encoding < %s \
; RUN:     | FileCheck -check-prefix=CHECK-FIXUP %s
; RUN: llvm-mc -filetype=obj -triple=mcs6502 < %s \
; RUN:     | llvm-objdump -d - | FileCheck -check-prefix=CHECK-INST %s
; RUN: llvm-mc -filetype=obj -triple=mcs6502 %s \
; RUN:     | llvm-readobj -r | FileCheck -check-prefix=CHECK-REL %s

; Checks that fixups that can be reoslved within the same object file are
; applied correctly.

tay
; CHECK-INST: tay

lda val16
; CHECK-FIXUP: fixup A - offset: 0, value : val16, kind: fixup_mcs6502_symbol16
sta val8
; CHECK-FIXUP: fixup A - offset: 0, value : val8, kind: fixup_mcs6502_symbol16
ldx #val16
; CHECK-FIXUP: fixup A - offset: 0, value : val16, kind: fixup_mcs6502_symbol8
ldy #val8
; CHECK-FIXUP: fixup A - offset: 0, value : val8, kind: fixup_mcs6502_symbol8

.LBB0:
tay

bcc .LBB0
; CHECK-INST: bcc #-1
; CHECK-FIXUP: fixup A - offset: 0, value : .LBB0, kind: fixup_mcs6502_symbol8

bcs .LBB1
; CHECK-INST: bcs #18
; CHECK-FIXUP: fixup A - offset: 0, value : .LBB1, kind: fixup_mcs6502_symbol8

.fill 16
.LBB1:
tay

.set val16, 0x1234
.set val8, 0xAB

; Test that all relocations resolved:
; CHECK-REL-NOT: R_MCS6502

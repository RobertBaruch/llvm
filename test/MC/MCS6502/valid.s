; RUN: llvm-mc %s -triple=mcs6502 -show-encoding \
; RUN:     | FileCheck -check-prefixes=CHECK,CHECK-INST %s
; RUN: llvm-mc -filetype=obj -triple=mcs6502 < %s \
; RUN:     | llvm-objdump -d - | FileCheck -check-prefixes=CHECK-INST %s

; CHECK-INST: tay
; CHECK: encoding: [0xa8]
tay
; CHECK-INST: lda #0xa8
; CHECK: encoding: [0xa9,0xa8]
lda #0xa8
; CHECK-INST: lda 0x6677
; CHECK: encoding: [0xad,0x77,0x66]
lda 0x6677
; CHECK-INST: lda 0x88
; CHECK: encoding: [0xa5,0x88]
lda 0x88
; CHECK-INST: lda 0x88,X
; CHECK: encoding: [0xb5,0x88]
lda 0x88,X
; CHECK-INST: lda 0x6677,X
; CHECK: encoding: [0xbd,0x77,0x66]
lda 0x6677,X
; CHECK-INST: lda 0x6677,Y
; CHECK: encoding: [0xb9,0x77,0x66]
lda 0x6677,Y
; CHECK-INST: jmp 0x6677
; CHECK: encoding: [0x4c,0x77,0x66]
jmp 0x6677
; CHECK-INST: jmp 0x88
; CHECK: encoding: [0x4c,0x88,0x00]
jmp 0x88
; CHECK-INST: jmp ( 0x6677 )
; CHECK: encoding: [0x6c,0x77,0x66]
jmp (0x6677)
; CHECK-INST: lda ( 0x6677,X )
; CHECK: encoding: [0xa1,0x77,0x66]
lda (0x6677,X)
; CHECK-INST: lda ( 0x6677 ),Y
; CHECK: encoding: [0xb1,0x77,0x66]
lda (0x6677),Y
; CHECK-INST: bcc #-1
; CHECK: encoding: [0x90,0xff]
bcc #-1

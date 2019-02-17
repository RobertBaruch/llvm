; RUN: llvm-mc %s -triple=mcs6502 -show-encoding \
; RUN:     | FileCheck -check-prefixes=CHECK,CHECK-INST %s
; RUN: llvm-mc %s -triple=mcs6502 -filetype=obj \
; RUN:     | llvm-objdump -d - | FileCheck -check-prefixes=CHECK-INST %s

; Immediate mode

; CHECK-INST: ADC #0xff
; CHECK: encoding: [0x69,0xff]
adc #0xFF

; CHECK-INST: ADC #0xfe
; CHECK: encoding: [0x69,0xfe]
adc #-2

; adc /0x1234

; zero-page

; CHECK-INST: ADC 0x36
; CHECK: encoding: [0x65,0x36]
adc 0x36

; absolute

; CHECK-INST: ADC 0x1234
; CHECK: encoding: [0x6d,0x34,0x12]
adc 0x1234

; pre-indirect (zp, X)

; CHECK-INST: ADC (0x12, X)
; CHECK: encoding: [0x61,0x12]
adc (0x12,X)

; CHECK-INST: ADC (0x13, X)
; CHECK: encoding: [0x61,0x13]
adc (0x12+1,X)

; post-indirect (zp), Y

; CHECK-INST: ADC (0x12), Y
; CHECK: encoding: [0x71,0x12]
adc (0x12),Y

; absolute, X

; CHECK-INST: ADC 0x1234, X
; CHECK: encoding: [0x7d,0x34,0x12]
adc 0x1234,X

; absolute, Y

; CHECK-INST: ADC 0x1234, Y
; CHECK: encoding: [0x79,0x34,0x12]
adc 0x1234, Y

; zp, X

; CHECK-INST: ADC 0x67
; CHECK: encoding: [0x75,0x67]
adc 0x67, X

; other alu-type instructions

; CHECK-INST: ORA 0x36
; CHECK: encoding: [0x05,0x36]
ora 0x36

; CHECK-INST: AND 0x4567
; CHECK: encoding: [0x2d,0x67,0x45]
and 0x4567

; CHECK-INST: EOR (0x33), Y
; CHECK: encoding: [0x51,0x33]
eor (0x33),Y

; CHECK-INST: LDA 0x1122, X
; CHECK: encoding: [0xbd,0x22,0x11]
lda 0x1122,X

; CHECK-INST: CMP #0xae
; CHECK: encoding: [0xc9,0xae]
cmp #0xAE

; CHECK-INST: SBC (0x1, X)
; CHECK: encoding: [0xe1,0x01]
sbc (0x01, X)

; sta

; CHECK-INST: STA 0x12
; CHECK: encoding: [0x85,0x12]
sta 0x12

; asl_group

; CHECK-INST: ROL 0x45
; CHECK: encoding: [0x26,0x45]
rol 0x45

; CHECK-INST: ASL A
; CHECK: encoding: [0x0a]
asl A

; CHECK-INST: LSR 0x9876
; CHECK: encoding: [0x4e,0x76,0x98]
lsr 0x9876

; CHECK-INST: ROR 0x77, X
; CHECK: encoding: [0x76,0x77]
ror 0x77,X

; CHECK-INST: DEC 0x4455, X
; CHECK: encoding: [0xde,0x55,0x44]
dec 0x4455,X

; inc/dec

; CHECK-INST: INC 0x12
; CHECK: encoding: [0xe6,0x12]
inc 0x12

; CHECK-INST: DEC 0x7654, X
; CHECK: encoding: [0xde,0x54,0x76]
dec 0x7654,X

; ldx

; CHECK-INST: LDX #0x33
; CHECK: encoding: [0xa2,0x33]
ldx #0x33

; stx

; CHECK-INST: STX 0x11, Y
; CHECK: encoding: [0x96,0x11]
stx 0x11,Y

; bit

; CHECK-INST: BIT 0x45
; CHECK: encoding: [0x24,0x45]
bit 0x45

; jmp

; CHECK-INST: JMP 0x2345
; CHECK: encoding: [0x4c,0x45,0x23]
jmp 0x2345

; jmp ind

; CHECK-INST: JMP (0x2345)
; CHECK: encoding: [0x6c,0x45,0x23]
jmp (0x2345)

; sty

; CHECK-INST: STY 0x12
; CHECK: encoding: [0x84,0x12]
sty 0x12

; ldy

; CHECK-INST: LDY #0xee
; CHECK: encoding: [0xa0,0xee]
ldy #0xEE

; cpx/cpy

; CHECK-INST: CPX 0x23
; CHECK: encoding: [0xe4,0x23]
cpx 0x23

; CHECK-INST: CPY 0x2345
; CHECK: encoding: [0xcc,0x45,0x23]
cpy 0x2345

; branch

; CHECK-INST: BPL 0xfe
; CHECK: encoding: [0x10,0xfe]
bpl -2

; jsr

; CHECK-INST: JSR 0x9800
; CHECK: encoding: [0x20,0x00,0x98]
jsr 0x9800

; brk

; CHECK-INST: BRK
; CHECK: encoding: [0x00]
brk

; comment
; CHECK-INST: BRK
; CHECK: encoding: [0x00]
brk ; comment
; CHECK-INST: ADC (0x12, X)
; CHECK: encoding: [0x61,0x12]
adc (0x12,X) ; comment

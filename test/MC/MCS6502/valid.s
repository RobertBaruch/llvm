; RUN: llvm-mc %s -triple=mcs6502 -show-encoding \
; RUN:     | FileCheck -check-prefixes=CHECK,CHECK-INST %s

; CHECK-INST: adc (0x1234,X)
; CHECK: encoding: [0xa8]
adc (0x1234,X)

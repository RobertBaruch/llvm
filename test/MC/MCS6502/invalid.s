; RUN: not llvm-mc -triple=mcs6502 < %s 2>&1 | FileCheck %s

; CHECK: :[[@LINE+1]]:13: error: invalid operand for instruction
adc (0x1234,Y) 
; CHECK: :[[@LINE+1]]:6: error: invalid operand for instruction
sty (0x1234,X) 
; CHECK: :[[@LINE+2]]:6: error: unknown token in expression
; CHECK: :[[@LINE+1]]:6: error: Failed to parse expression
adc (
; CHECK: :[[@LINE+1]]:12: error: Expected ',' or ')' after expression
adc (0x1234
; CHECK: :[[@LINE+1]]:13: error: expected register name
adc (0x1234,
; CHECK: :[[@LINE+1]]:14: error: Expected ')' after indirect value
adc (0x1234,X
; CHECK: :[[@LINE+1]]:1: error: too few operands for instruction
asl

; RUN: not llvm-mc -triple=mcs6502 -filetype=obj < %s -o /dev/null 2>&1 | FileCheck %s

.TOO_FAR_BACK:
.fill 256

; Out of range branch
bcs .TOO_FAR_BACK ; CHECK: [[@LINE]]:1: error: Fixup value for branch out of range

; Out of range branch
bcs .TOO_FAR_AHEAD ; CHECK: [[@LINE]]:1: error: Fixup value for branch out of range

.fill 256
.TOO_FAR_AHEAD:
tay

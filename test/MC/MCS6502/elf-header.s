# RUN: llvm-mc %s -filetype=obj -triple=mcs6502 | llvm-readobj -h \
# RUN:     | FileCheck -check-prefix=X %s

# X: Format: ELF32-mcs6502
# X: Arch: mcs6502
# X: AddressSize: 32bit
# X: ElfHeader {
# X:   Ident {
# X:     Magic: (7F 45 4C 46)
# X:     Class: 32-bit (0x1)
# X:     DataEncoding: LittleEndian (0x1)
# X:     FileVersion: 1
# X:     OS/ABI: SystemV (0x0)
# X:     ABIVersion: 0
# X:   }
# X:   Type: Relocatable (0x1)
# X:   Machine: EM_MCS6502 (0xFE)
# X:   Version: 1
# X:   Flags [ (0x0)
# X:   ]
# X: }

set(LLVM_TARGET_DEFINITIONS MCS6502.td)

tablegen(LLVM MCS6502GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MCS6502GenInstrInfo.inc -gen-instr-info)

add_public_tablegen_target(MCS6502CommonTableGen)

add_llvm_target(MCS6502CodeGen
  MCS6502TargetMachine.cpp
  )

add_subdirectory(TargetInfo)

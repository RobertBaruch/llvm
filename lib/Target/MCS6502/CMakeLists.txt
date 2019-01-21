set(LLVM_TARGET_DEFINITIONS MCS6502.td)

tablegen(LLVM MCS6502GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MCS6502GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM MCS6502GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM MCS6502GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM MCS6502GenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM MCS6502GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM MCS6502GenDisassemblerTables.inc -gen-disassembler)

add_public_tablegen_target(MCS6502CommonTableGen)

add_llvm_target(MCS6502CodeGen
  MCS6502TargetMachine.cpp
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)

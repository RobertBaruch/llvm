//===-- MCS6502MCTargetDesc.h - MCS6502 Target Descriptions -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides MCS6502 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502MCTARGETDESC_H
#define LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502MCTARGETDESC_H

#include "llvm/Config/config.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {

class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class StringRef;
class Target;
class Triple;
class raw_ostream;
class raw_pwrite_stream;

Target &getTheMCS6502Target();

MCCodeEmitter *createMCS6502MCCodeEmitter(const MCInstrInfo &MCII,
                                          const MCRegisterInfo &MRI,
                                          MCContext &Ctx);

MCAsmBackend *createMCS6502AsmBackend(const Target &T,
                                      const MCSubtargetInfo &STI,
                                      const MCRegisterInfo &MRI,
                                      const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter>
createMCS6502ELFObjectWriter(uint8_t OSABI, bool Is64Bit);

} // namespace llvm

// Defines symbolic names for MCS6502 registers.
#define GET_REGINFO_ENUM
#include "MCS6502GenRegisterInfo.inc"

// Defines symbolic names for MCS6502 instructions.
#define GET_INSTRINFO_ENUM
#include "MCS6502GenInstrInfo.inc"

#endif // LLVM_LIB_TARGET_MCS6502_MCTARGET_DESC_MCS6502MCTARGETDESC_H
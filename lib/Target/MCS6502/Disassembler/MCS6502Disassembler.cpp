//===- MCS6502Disassembler.cpp - Disassembler for MCS6502 -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements MCS6502Disassembler.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MCS6502MCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
// Not really fixed-len, just has enums.
#include "llvm/MC/MCFixedLenDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/NativeFormatting.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mcs6502-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

namespace {

class MCS6502Disassembler : public MCDisassembler {
public:
  MCS6502Disassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}

  DecodeStatus getInstruction(MCInst &MI, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &VStream,
                              raw_ostream &CStream) const override;
};

} // namespace

static MCDisassembler *createMCS6502Disassembler(const Target &T,
                                                 const MCSubtargetInfo &STI,
                                                 MCContext &Ctx) {
  return new MCS6502Disassembler(STI, Ctx);
}

extern "C" void LLVMInitializeMCS6502Disassembler() {
  TargetRegistry::RegisterMCDisassembler(getTheMCS6502Target(),
                                         createMCS6502Disassembler);
}

template <unsigned N>
static DecodeStatus decodeUImmOperand(MCInst &Inst, uint64_t Imm,
                                      int64_t Address, const void *Decoder) {
  assert(isUInt<N>(Imm) && "Invalid immediate");
  LLVM_DEBUG(dbgs() << "DecodeUImmOperand " << N << "\n");
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

template <unsigned N>
static DecodeStatus decodeSImmOperand(MCInst &Inst, uint64_t Imm,
                                      int64_t Address, const void *Decoder) {
  assert(isUInt<N>(Imm) && "Invalid immediate");
  Inst.addOperand(MCOperand::createImm(SignExtend64<N>(Imm)));
  return MCDisassembler::Success;
}

#include "MCS6502GenDisassemblerTables.inc"

DecodeStatus MCS6502Disassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                                 ArrayRef<uint8_t> Bytes,
                                                 uint64_t Address,
                                                 raw_ostream &VStream,
                                                 raw_ostream &CStream) const {
  uint32_t Inst = support::endian::read32be(Bytes.data());
  LLVM_DEBUG(dbgs() << "Inst8: ");
  LLVM_DEBUG(llvm::write_hex(dbgs(), Inst >> 24, HexPrintStyle::PrefixLower));
  LLVM_DEBUG(dbgs() << "\n");
  Size = 1;
  DecodeStatus status =
      decodeInstruction(DecoderTable8, MI, Inst >> 24, Address, this, STI);
  if (status == MCDisassembler::Success) {
    LLVM_DEBUG(dbgs() << "OK!\n");
    return status;
  }

  LLVM_DEBUG(dbgs() << "No? How about Inst16: ");
  LLVM_DEBUG(llvm::write_hex(dbgs(), Inst >> 16, HexPrintStyle::PrefixLower));
  LLVM_DEBUG(dbgs() << "\n");
  Size = 2;
  status =
      decodeInstruction(DecoderTable16, MI, Inst >> 16, Address, this, STI);
  if (status == MCDisassembler::Success)
    return status;

  LLVM_DEBUG(dbgs() << "No? How about Inst32: ");
  LLVM_DEBUG(llvm::write_hex(dbgs(), Inst >> 8, HexPrintStyle::PrefixLower));
  LLVM_DEBUG(dbgs() << "\n");
  Size = 3;
  return decodeInstruction(DecoderTable24, MI, Inst >> 8, Address, this, STI);
}

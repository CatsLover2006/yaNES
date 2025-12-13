//
//  CPU.cpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#include "Common.hpp"
#include "CPU.hpp"

#include "InstructionSet.h"

#include <iostream>

#define read16(memory, address) (memory.read(address) + (memory.read(address + 1) << 8))

#define FINISHED_INSTRUCTION (-1)

#define statusC (status & 0b00000001)
#define statusZ (status & 0b00000010)
#define statusI (status & 0b00000100)
#define statusD (status & 0b00001000)
#define statusB (status & 0b00010000)
#define statusV (status & 0b01000000)
#define statusN (status & 0b10000000)

#define setStatusC status |= 0b00000001
#define setStatusZ status |= 0b00000010
#define setStatusI status |= 0b00000100
#define setStatusD status |= 0b00001000
#define setStatusB status |= 0b00010000
#define setStatusV status |= 0b01000000
#define setStatusN status |= 0b10000000

#define clearStatusC status &= 0b11111110
#define clearStatusZ status &= 0b11111101
#define clearStatusI status &= 0b11111011
#define clearStatusD status &= 0b11110111
#define clearStatusB status &= 0b11101111
#define clearStatusV status &= 0b10111111
#define clearStatusN status &= 0b01111111

#define updateZ(val)    if (val) clearStatusZ;\
                        else setStatusZ

#define updateN(val)    if (val & 0x80) setStatusN;\
                        else clearStatusN

#define updateZN(val)   updateN(val);\
                        updateZ(val)

#define sp stack

// Magic constant for illegal opcodes
#define MAGIC 0x69

CPU::CPU(Memory &memory) : memory(memory) {
    accumulator = 0;
    x = 0;
    y = 0;
    stack = 0xff;
    status = 0;
}

void CPU::setReset() {
    resetCPU = true;
}

void CPU::clearReset() {
    resetCPU = false;
}

void CPU::setNMI() {
    recievedNMI = true;
}

void CPU::clearNMI() {
    recievedNMI = false;
}

void CPU::setIRQ() {
    recievedIRQ = true;
}

void CPU::clearIRQ() {
    recievedIRQ = false;
}

void CPU::doCycle() {
    if (resetCPU) {
        subCycle = 0;
        recievedInterrupt = true;
    }
    if (subCycle == 0) {
        // TODO: full instruction decoding logic
        t = recievedInterrupt;
        if (recievedInterrupt) instruction = BRK;
        else instruction = memory.read(pc);
        pc++;
    } else doInstruction();
    if (subCycle == FINISHED_INSTRUCTION)
        recievedInterrupt = recievedNMI || recievedIRQ;
    subCycle++;
}

// Instruction Macros

#define macro_impl(inst) {\
    memory.read(pc);\
    inst;\
    subCycle = FINISHED_INSTRUCTION;\
    return;}

#define macro_imm(inst) {\
    t = memory.read(pc);\
    inst;\
    pc++;\
    subCycle = FINISHED_INSTRUCTION;\
    return;}

#define macro_zero_read(inst) {\
    switch(subCycle) {\
        case 1: {\
            t = memory.read(pc);\
            pc++;\
            return;\
        }\
        case 2: {\
            t = memory.read(t);\
            inst;\
            subCycle = FINISHED_INSTRUCTION;\
            return;\
        }\
    }

#define macro_zero_indexed_read(inst, index) {\
    switch(subCycle) {\
        case 1: {\
            t = memory.read(pc);\
            pc++;\
            return;\
        }\
        case 2: {\
            memory.read(t);\
            t += index;\
            return;\
        }\
        case 3: {\
            t = memory.read(t);\
            inst;\
            subCycle = FINISHED_INSTRUCTION;\
            return;\
        }\
    }

void CPU::doInstruction() {
    // TODO: implement instructions
    // https://www.nesdev.org/6502_cpu.txt
    switch (instruction) {
        case BRK: {
            // TODO: do this right
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    break;
                }
                case 2: {
                    t = status;
                    if (recievedNMI || resetCPU) t = 0;
                    if (t & 0b00010000) stackPush(pc >> 8);
                    break;
                }
                case 3: {
                    if (t & 0b00010000) stackPush(pc & 0xff);
                    break;
                }
                case 4: {
                    if (t & 0b00010000) stackPush(status);
                    break;
                }
                case 5: {
                    if (resetCPU) t = memory.read(0xfffc);
                    else if (recievedNMI) {
                        t = memory.read(0xfffa);
                    } else t = memory.read(0xfffe); // IRQ or BRK
                }
                case 6: {
                    if (resetCPU) pc = memory.read(0xfffd) << 8 | t;
                    else if (recievedNMI) {
                        pc = memory.read(0xfffb) << 8 | t;
                    } else pc = memory.read(0xffff) << 8 | t; // IRQ or BRK
                    recievedInterrupt = false;
                    subCycle = FINISHED_INSTRUCTION;
                    break;
                }
            }
            return;
        }
        case ASL:
        case ROL: {
            memory.read(pc);
            if (accumulator & 0x80) t = true;
            else t = false;
            accumulator <<= 1;
            if (instruction == ROL && statusC) accumulator |= 1;
            if (t) setStatusC;
            else clearStatusC;
            updateZN(accumulator);
            subCycle = FINISHED_INSTRUCTION;
            return;
        }
        case ROR: {
            memory.read(pc);
            if (accumulator & 1) t = true;
            else t = false;
            accumulator >>= 1;
            if (statusC) accumulator |= 0x80;
            if (t) setStatusC;
            else clearStatusC;
            updateZN(accumulator);
            subCycle = FINISHED_INSTRUCTION;
            return;
        }
        case INX: macro_impl(x++; updateZN(x));
        case INY: macro_impl(y++; updateZN(y));
        case DEX: macro_impl(x--; updateZN(x));
        case DEY: macro_impl(y--; updateZN(y));
        case CLC: macro_impl(clearStatusC);
        case CLD: macro_impl(clearStatusD);
        case CLI: macro_impl(clearStatusI);
        case CLV: macro_impl(clearStatusV);
        case SEC: macro_impl(setStatusC);
        case SED: macro_impl(setStatusD);
        case SEI: macro_impl(setStatusI);
        case TAX: macro_impl(x = accumulator; updateZN(accumulator));
        case TAY: macro_impl(y = accumulator; updateZN(accumulator));
        case TSX: macro_impl(x = stack; updateZN(x));
        case TXS: macro_impl(stack = x);
        case TXA: macro_impl(accumulator = x; updateZN(accumulator));
        case TYA: macro_impl(accumulator = y; updateZN(accumulator));
        case ADC:
        case SBC: macro_imm({
            if (instruction == SBC) t = ~t;
            t16 = accumulator;
            t16 += t;
            if (statusC) t16++;
            if (t16 & 0xff00) setStatusC;
            else clearStatusC;
            // TODO: overflow flag
            accumulator = t16 & 0xff;
        });
        case AND: macro_imm(accumulator &= t; updateZN(accumulator));
        case AND_0: macro_zero_read(accumulator &= t; updateZN(accumulator));
        case AND_0X: macro_zero_indexed_read(accumulator &= t; updateZN(accumulator), x);
        case ORA: macro_imm(accumulator |= t; updateZN(accumulator));
        case ORA_0: macro_zero_read(accumulator |= t; updateZN(accumulator));
        case ORA_0X: macro_zero_indexed_read(accumulator |= t; updateZN(accumulator), x);
        case CMP: macro_imm({
            updateZN(accumulator - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case CPX: macro_imm({
            updateZN(x - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case CPY: macro_imm({
            updateZN(y - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case EOR: macro_imm(accumulator ^= t; updateZN(accumulator));
        case LDA: macro_imm(accumulator = t; updateZN(t));
        case LDX: macro_imm(x = t; updateZN(t));
        case LDY: macro_imm(y = t; updateZN(t));
        case LXA: macro_imm(x = (accumulator | MAGIC) & t; accumulator = x; updateZN(x));
        case ALR: {
            accumulator &= memory.read(pc);
            if (accumulator & 1) t = true;
            else t = false;
            accumulator >>= 1;
            if (t) setStatusC;
            else clearStatusC;
            updateZN(accumulator);
            subCycle = FINISHED_INSTRUCTION;
            return;
        }
        case ANC:
        case ANC_R: {
            accumulator &= memory.read(pc);
            if (accumulator & 0x80) setStatusC;
            else clearStatusC;
            updateZN(accumulator);
            subCycle = FINISHED_INSTRUCTION;
            return;
        }
        case XAA: macro_imm(accumulator = (accumulator | MAGIC) & x & t; updateZN(accumulator));
        case ARR: {
            accumulator &= memory.read(pc);
            if (accumulator & 0x80) t = true;
            else t = false;
            accumulator >>= 1;
            if (statusC) accumulator |= 0x80;
            if (t) setStatusC;
            else clearStatusC;
            updateZN(accumulator);
            subCycle = FINISHED_INSTRUCTION;
            return;
        }
        case 0x1A:
        case 0x3A:
        case 0x5A:
        case 0x7A:
        case 0xDA: // Implied NOP
        case 0xFA: macro_impl();
        case 0x80:
        case 0x82:
        case 0x89:
        case 0xC2: // Immediate NOP
        case 0xE2: macro_imm();
        case 0x04:
        case 0x44: // Zeropage NOP
        case 0x64: macro_zero_read();
        case 0x14:
        case 0x34:
        case 0x54:
        case 0x74:
        case 0xD4: // Zeropage Indexed NOP
        case 0xF4: macro_zero_indexed_read(,0);
        case 0x02:
        case 0x12:
        case 0x22:
        case 0x32:
        case 0x42:
        case 0x52:
        case 0x62:
        case 0x72:
        case 0x92:
        case 0xB2:
        case 0xD2:
        case 0xF2: { // JAM
            subCycle = 50;
            return;
        }
    }
}

void CPU::stackPush(u8 value) {
    memory.write(0x100 + stack, value);
    stack--;
}

u8 CPU::stackPop() {
    u8 value = memory.read(0x100 + stack);
    stack++;
    return value;
}

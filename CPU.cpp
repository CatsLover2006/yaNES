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
    }\
}

#define macro_abs_read(inst) {\
    switch(subCycle) {\
        case 1: {\
            t = memory.read(pc);\
            pc++;\
            return;\
        }\
        case 2: {\
            t16 = memory.read(pc) << 8 | t;\
            pc++;\
            return;\
        }\
        case 3: {\
            t = memory.read(t16);\
            inst;\
            subCycle = FINISHED_INSTRUCTION;\
            return;\
        }\
    }\
}

#define macro_abs_indexed_read(inst, index) {\
    switch(subCycle) {\
        case 1: {\
            t16 = memory.read(pc);\
            pc++;\
            return;\
        }\
        case 2: {\
            t = memory.read(pc);\
            t16 += t << 8;\
            pc++;\
            return;\
        }\
        case 3: {\
            t16 += index;\
            if (t16 >> 8 != t) {\
                memory.read(t16 - 0x0100);\
                return;\
            }\
        }\
        case 4: {\
            t = memory.read(t16);\
            inst;\
            subCycle = FINISHED_INSTRUCTION;\
            return;\
        }\
    }\
}
void CPU::doInstruction() {
    // TODO: implement instructions
    // https://www.nesdev.org/6502_cpu.txt
    switch (instruction) {
        case BRK: // TODO: fix interrupt integration
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    return;
                }
                case 2: {
                    t = status;
                    if (recievedNMI || resetCPU) t = 0;
                    if (t & 0b00010000) stackPush(pc >> 8);
                    return;
                }
                case 3: {
                    if (t & 0b00010000) stackPush(pc & 0xff);
                    return;
                }
                case 4: {
                    if (t & 0b00010000) stackPush(status);
                    return;
                }
                case 5: {
                    if (resetCPU) t = memory.read(0xfffc);
                    else if (recievedNMI) {
                        t = memory.read(0xfffa);
                    } else t = memory.read(0xfffe); // IRQ or BRK
                    return;
                }
                case 6: {
                    if (resetCPU) pc = memory.read(0xfffd) << 8 | t;
                    else if (recievedNMI) {
                        pc = memory.read(0xfffb) << 8 | t;
                    } else pc = memory.read(0xffff) << 8 | t; // IRQ or BRK
                    recievedInterrupt = false;
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case PHA:
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    return;
                }
                case 2: {
                    stackPush(accumulator);
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case PHP:
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    return;
                }
                case 2: {
                    stackPush(sp | 0x30);
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case PLA:
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    return;
                }
                case 3: {
                    accumulator = stackPop();
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case PLP:
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    return;
                }
                case 3: {
                    sp = stackPop() & 0xCF;
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case JMP:
            switch (subCycle) {
                case 1: {
                    t = memory.read(pc);
                    pc++;
                    return;
                }
                case 2: {
                    pc = (memory.read(pc) << 8) + t;
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case JMP_I:
            switch (subCycle) {
                case 1: {
                    t = memory.read(pc);
                    pc++;
                    return;
                }
                case 2: {
                    t16 = (memory.read(pc) << 8) + t;
                    return;
                }
                case 3: {
                    t = memory.read(t16);
                    return;
                }
                case 4: {
                    pc = t16;
                    t16++;
                    if ((t16 & 0xff00) != (pc & 0xff00)) t16 -= 0x0100;
                    pc = (memory.read(t16) << 8) + t;
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case JSR:
            switch (subCycle) {
                case 1: {
                    t = memory.read(pc);
                    pc++;
                    return;
                }
                case 2: {
                    memory.read(0x100 + stack);
                    return;
                }
                case 3: {
                    stackPush(pc >> 8);
                    return;
                }
                case 4: {
                    stackPush(pc & 0xff);
                    return;
                }
                case 5: {
                    pc = (memory.read(pc) << 8) + t;
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case RTS:
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    return;
                }
                case 3: {
                    t = stackPop();
                    return;
                }
                case 4: {
                    pc = (stackPop() << 8) + t;
                    return;
                }
                case 5: {
                    pc++;
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case RTI:
            switch (subCycle) {
                case 1: {
                    memory.read(pc);
                    return;
                }
                case 3: {
                    status = stackPop();
                    return;
                }
                case 4: {
                    t = stackPop();
                    return;
                }
                case 5: {
                    pc = (stackPop() << 8) + t;
                    subCycle = FINISHED_INSTRUCTION;
                    return;
                }
            }
        case ASL:
        case ROL: macro_impl({
            if (accumulator & 0x80) t = true;
            else t = false;
            accumulator <<= 1;
            if (instruction == ROL && statusC) accumulator |= 1;
            if (t) setStatusC;
            else clearStatusC;
            updateZN(accumulator);
        });
        case ROR: macro_impl({
            if (accumulator & 1) t = true;
            else t = false;
            accumulator >>= 1;
            if (statusC) accumulator |= 0x80;
            if (t) setStatusC;
            else clearStatusC;
            updateZN(accumulator);
        });
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
        case SBC:
        case USBC: macro_imm({
            if (instruction != ADC) t = ~t;
            t16 = accumulator;
            t16 += t;
            if (statusC) t16++;
            if (t16 & 0xff00) setStatusC;
            else clearStatusC;
            t16 &= 0x00ff;
            if ((accumulator ^ t16) & (t ^ t16) & 0x80) setStatusV;
            else clearStatusV;
            accumulator = t16;
        });
        case AND: macro_imm(accumulator &= t; updateZN(accumulator));
        case ORA: macro_imm(accumulator |= t; updateZN(accumulator));
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
        case ADC_0:
        case SBC_0: macro_zero_read({
            if (instruction == SBC_0) t = ~t;
            t16 = accumulator;
            t16 += t;
            if (statusC) t16++;
            if (t16 & 0xff00) setStatusC;
            else clearStatusC;
            t16 &= 0x00ff;
            if ((accumulator ^ t16) & (t ^ t16) & 0x80) setStatusV;
            else clearStatusV;
            accumulator = t16 & 0xff;
        });
        case AND_0: macro_zero_read(accumulator &= t; updateZN(accumulator));
        case ORA_0: macro_zero_read(accumulator |= t; updateZN(accumulator));
        case CMP_0: macro_zero_read({
            updateZN(accumulator - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case CPX_0: macro_zero_read({
            updateZN(x - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case CPY_0: macro_zero_read({
            updateZN(y - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case EOR_0: macro_zero_read(accumulator ^= t; updateZN(accumulator));
        case LDA_0: macro_zero_read(accumulator = t; updateZN(t));
        case LDX_0: macro_zero_read(x = t; updateZN(t));
        case LDY_0: macro_zero_read(y = t; updateZN(t));
        case LAX_0: macro_zero_read(x = (accumulator | MAGIC) & t; accumulator = x; updateZN(x));
        case ADC_0X:
        case SBC_0X: macro_zero_indexed_read({
            if (instruction == SBC_0) t = ~t;
            t16 = accumulator;
            t16 += t;
            if (statusC) t16++;
            if (t16 & 0xff00) setStatusC;
            else clearStatusC;
            t16 &= 0x00ff;
            if ((accumulator ^ t16) & (t ^ t16) & 0x80) setStatusV;
            else clearStatusV;
            accumulator = t16 & 0xff;
        }, x);
        case AND_0X: macro_zero_indexed_read(accumulator &= t; updateZN(accumulator), x);
        case ORA_0X: macro_zero_indexed_read(accumulator |= t; updateZN(accumulator), x);
        case CMP_0X: macro_zero_indexed_read({
            updateZN(accumulator - t);
            if (statusN) clearStatusC;
            else setStatusC;
        }, x);
        case EOR_0X: macro_zero_indexed_read(accumulator ^= t; updateZN(accumulator), x);
        case LDA_0X: macro_zero_indexed_read(accumulator = t; updateZN(t), x);
        case LDX_0Y: macro_zero_indexed_read(x = t; updateZN(t), y);
        case LDY_0X: macro_zero_indexed_read(y = t; updateZN(t), x);
        case LAX_0Y: macro_zero_indexed_read(x = (accumulator | MAGIC) & t; accumulator = x; updateZN(x), y);
        case ADC_A:
        case SBC_A: macro_abs_read({
            if (instruction == SBC_A) t = ~t;
            t16 = accumulator;
            t16 += t;
            if (statusC) t16++;
            if (t16 & 0xff00) setStatusC;
            else clearStatusC;
            t16 &= 0x00ff;
            if ((accumulator ^ t16) & (t ^ t16) & 0x80) setStatusV;
            else clearStatusV;
            accumulator = t16 & 0xff;
        });
        case AND_A: macro_abs_read(accumulator &= t; updateZN(accumulator));
        case ORA_A: macro_abs_read(accumulator |= t; updateZN(accumulator));
        case CMP_A: macro_abs_read({
            updateZN(accumulator - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case CPX_A: macro_abs_read({
            updateZN(x - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case CPY_A: macro_abs_read({
            updateZN(y - t);
            if (statusN) clearStatusC;
            else setStatusC;
        });
        case EOR_A: macro_abs_read(accumulator ^= t; updateZN(accumulator));
        case LDA_A: macro_abs_read(accumulator = t; updateZN(t));
        case LDX_A: macro_abs_read(x = t; updateZN(t));
        case LDY_A: macro_abs_read(y = t; updateZN(t));
        case LAX_A: macro_abs_read(x = (accumulator | MAGIC) & t; accumulator = x; updateZN(x));
        case ADC_AX:
        case SBC_AX: macro_abs_indexed_read({
            if (instruction == SBC_0) t = ~t;
            t16 = accumulator;
            t16 += t;
            if (statusC) t16++;
            if (t16 & 0xff00) setStatusC;
            else clearStatusC;
            t16 &= 0x00ff;
            if ((accumulator ^ t16) & (t ^ t16) & 0x80) setStatusV;
            else clearStatusV;
            accumulator = t16 & 0xff;
        }, x);
        case AND_AX: macro_abs_indexed_read(accumulator &= t; updateZN(accumulator), x);
        case ORA_AX: macro_abs_indexed_read(accumulator |= t; updateZN(accumulator), x);
        case CMP_AX: macro_abs_indexed_read({
            updateZN(accumulator - t);
            if (statusN) clearStatusC;
            else setStatusC;
        }, x);
        case EOR_AX: macro_abs_indexed_read(accumulator ^= t; updateZN(accumulator), x);
        case LDA_AX: macro_abs_indexed_read(accumulator = t; updateZN(t), x);
        case LDX_AY: macro_abs_indexed_read(x = t; updateZN(t), y);
        case LDY_AX: macro_abs_indexed_read(y = t; updateZN(t), x);
        case LAX_AY: macro_abs_indexed_read(x = (accumulator | MAGIC) & t; accumulator = x; updateZN(x), y);
        case ADC_AY:
        case SBC_AY: macro_abs_indexed_read({
            if (instruction == SBC_0) t = ~t;
            t16 = accumulator;
            t16 += t;
            if (statusC) t16++;
            if (t16 & 0xff00) setStatusC;
            else clearStatusC;
            t16 &= 0x00ff;
            if ((accumulator ^ t16) & (t ^ t16) & 0x80) setStatusV;
            else clearStatusV;
            accumulator = t16 & 0xff;
        }, y);
        case AND_AY: macro_abs_indexed_read(accumulator &= t; updateZN(accumulator), y);
        case ORA_AY: macro_abs_indexed_read(accumulator |= t; updateZN(accumulator), y);
        case CMP_AY: macro_abs_indexed_read({
            updateZN(accumulator - t);
            if (statusN) clearStatusC;
            else setStatusC;
        }, y);
        case EOR_AY: macro_abs_indexed_read(accumulator ^= t; updateZN(accumulator), y);
        case LDA_AY: macro_abs_indexed_read(accumulator = t; updateZN(t), y);
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
        case NOP:
        case 0x1A:
        case 0x3A:
        case 0x5A:
        case 0x7A: // Implied NOP
        case 0xDA: // 1 Byte, 2 Cycle
        case 0xFA: macro_impl();
        case 0x80:
        case 0x82:
        case 0x89: // Immediate NOP
        case 0xC2: // 2 Bytes, 2 Cycle
        case 0xE2: macro_imm();
        case 0x04: // Zeropage NOP
        case 0x44: // 2 Bytes, 3 Cycles
        case 0x64: macro_zero_read();
        case 0x14:
        case 0x34:
        case 0x54:
        case 0x74: // Zeropage Indexed NOP
        case 0xD4: // 2 Bytes, 4 Cycles
        case 0xF4: macro_zero_indexed_read(,x);
        case 0x0C: macro_abs_read(); // Absolute NOP; 3 Bytes, 4 Cycles
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C: // Absolute Indexed NOP
        case 0xDC: // 3 Bytes, 4/5 Cycles
        case 0xFC: macro_abs_indexed_read(,x);
        case 0x02:
        case 0x12:
        case 0x22:
        case 0x32:
        case 0x42:
        case 0x52:
        case 0x62:
        case 0x72:
        case 0x92: // JAM, KIL, HLT
        case 0xB2: // No need to accurately emulate
        case 0xD2: // the crash instructions
        case 0xF2: {
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
    stack++;
    u8 value = memory.read(0x100 + stack);
    return value;
}

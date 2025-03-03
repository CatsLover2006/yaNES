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

#define read16(memory, address) (memory.read(memory) + (memory.read(memory + 1) << 8)

#define doingInterrupt (recievedNMI || recievedIRQ)

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
#define magic 0x69

CPU::CPU(Memory &memory) : memory(memory) {
    accumulator = 0;
    x = 0;
    y = 0;
    stack = 0;
    status = 0;
}

void CPU::reset() {
    pc = memory.read(0xff00 + t) + (memory.read(0xff01 + t) << 8);
    subCycles = 0;
    setStatusI;
    recievedNMI = false;
    recievedIRQ = false;
    resetCPU = true;
}

void CPU::transmitNMI() {
    setStatusI;
    recievedNMI = true;
}

void CPU::transmitIRQ() {
    recievedIRQ |= !statusI; // Interrupt Disable
    setStatusI;
}

void CPU::doCycle() {
    if (!subCycles || resetCPU) {
        if (resetCPU) resetCPU = false;
        if (instruction == BRK) {
            recievedNMI = false;
            recievedIRQ = false;
        }
        if (doingInterrupt) instruction = BRK;
        else instruction = memory.read(pc);
        switch (instruction) {
                // TODO: set cycle count
            case BRK:
                if (doingInterrupt) setStatusB;
                else clearStatusB;
                subCycles = 7;
                break;
            case JSR:
            case RTI:
                subCycles = 6;
                break;
            case JMP_I:
                subCycles = 5;
                break;
            case LDA_0X:
            case LDX_0Y:
            case LAX_0Y:
            case LDY_0X:
            case LDA_A:
            case LDX_A:
            case LAX_A:
            case LDY_A:
            case LDA_AX:
            case LDA_AY:
            case LDX_AY:
            case LAX_AY:
            case LDY_AX:
            case STA_0X:
            case STX_0Y:
            case STY_0X:
            case EOR_0X:
            case 0x0C:
            case 0x14:
            case 0x1C:
            case 0x34:
            case 0x3C:
            case 0x54:
            case 0x5C:
            case 0x74:
            case 0x7C:
            case 0xD4:
            case 0xDC:
            case 0xF4:
            case 0xFC:
                subCycles = 4;
                break;
            case PHP:
            case PHA:
            case JMP:
            case LDA_0:
            case LDX_0:
            case LAX_0:
            case LDY_0:
            case STA_0:
            case STX_0:
            case STY_0:
            case EOR_0:
            case 0x04:
            case 0x44:
            case 0x64:
                subCycles = 3;
                break;
            case SEI:
            case SED:
            case SEC:
            case CLI:
            case CLD:
            case CLC:
            case CLV:
            case LDA:
            case LDX:
            case LXA:
            case LDY:
            case DEX:
            case DEY:
            case INX:
            case INY:
            case ROL:
            case ROR:
            case ASL:
            case TAX:
            case TAY:
            case TSX:
            case TXA:
            case TYA:
            case TXS:
            case NOP:
            case EOR:
            case ANC:
            case ANC_R:
            default:
                subCycles = 2;
                break;
        }
    }
    doInstruction();
}

void CPU::doInstruction() {
    switch (instruction) { // TODO: implement instructions
        case LDA_AX:
        case LDA_AY: {
            switch (subCycles) {
                case 9:
                    subCycles = 1;
                    break;
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    t = t16 >> 8;
                    if (instruction == LDA_AX) t16 += x;
                    else t16 += y;
                    if (t != t16 >> 8) subCycles = 10;
                    accumulator = memory.read(t16);
                    updateZN(accumulator);
                    pc += 3;
                    break;
            }
            break;
        }
        case LAX_AY: {
            switch (subCycles) {
                case 9:
                    subCycles = 1;
                    break;
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    t = t16 >> 8;
                    t16 += y;
                    if (t != t16 >> 8) subCycles = 10;
                    x = memory.read(t16);
                    accumulator = x;
                    updateZN(x);
                    pc += 3;
                    break;
            }
            break;
        }
        case LDX_AY: {
            switch (subCycles) {
                case 9:
                    subCycles = 1;
                    break;
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    t = t16 >> 8;
                    t16 += y;
                    if (t != t16 >> 8) subCycles = 10;
                    x = memory.read(t16);
                    updateZN(x);
                    pc += 3;
                    break;
            }
            break;
        }
        case LDY_AX: {
            switch (subCycles) {
                case 9:
                    subCycles = 1;
                    break;
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    t = t16 >> 8;
                    t16 += x;
                    if (t != t16 >> 8) subCycles = 10;
                    y = memory.read(t16);
                    updateZN(y);
                    pc += 3;
                    break;
            }
            break;
        }
        case LDA_A: {
            switch (subCycles) {
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    accumulator = memory.read(t16);
                    updateZN(accumulator);
                    pc += 3;
                    break;
            }
            break;
        }
        case LAX_A: {
            switch (subCycles) {
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    x = memory.read(t16);
                    accumulator = x;
                    updateZN(x);
                    pc += 3;
                    break;
            }
            break;
        }
        case LDX_A: {
            switch (subCycles) {
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    x = memory.read(t16);
                    updateZN(x);
                    pc += 3;
                    break;
            }
            break;
        }
        case LDY_A: {
            switch (subCycles) {
                case 3:
                    t16 = memory.read(pc + 1);
                    break;
                case 2:
                    t16 += memory.read(pc + 2) << 8;
                    break;
                case 1:
                    y = memory.read(t16);
                    updateZN(y);
                    pc += 3;
                    break;
            }
            break;
        }
        case LDA_0X:
        case LDA_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == LDA_0X) mem += x;
                accumulator = memory.read(mem);
                updateZN(accumulator);
                pc += 2;
            }
            break;
        }
        case LDX_0Y:
        case LDX_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == LDX_0Y) mem += y;
                x = memory.read(mem);
                updateZN(x);
                pc += 2;
            }
            break;
        }
        case LAX_0Y:
        case LAX_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == LAX_0Y) mem += y;
                x = memory.read(mem);
                accumulator = x;
                updateZN(x);
                pc += 2;
            }
            break;
        }
        case LDY_0X:
        case LDY_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == LDY_0X) mem += x;
                y = memory.read(mem);
                updateZN(y);
                pc += 2;
            }
            break;
        }
        case STA_0X:
        case STA_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == LDA_0X) mem += x;
                writeback.location = mem;
                writeback.data = accumulator;
                writeback.needsWrite = true;
                pc += 2;
            }
            break;
        }
        case STX_0Y:
        case STX_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == LDX_0Y) mem += y;
                writeback.location = mem;
                writeback.data = x;
                writeback.needsWrite = true;
                pc += 2;
            }
            break;
        }
        case STY_0X:
        case STY_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == LDY_0X) mem += x;
                writeback.location = mem;
                writeback.data = y;
                writeback.needsWrite = true;
                pc += 2;
            }
            break;
        }
        case LDA: {
            if (subCycles == 1) {
                accumulator = memory.read(pc + 1);
                updateZN(accumulator);
                pc += 2;
            }
            break;
        }
        case LDX: {
            if (subCycles == 1) {
                x = memory.read(pc + 1);
                updateZN(x);
                pc += 2;
            }
            break;
        }
        case LDY: {
            if (subCycles == 1) {
                y = memory.read(pc + 1);
                updateZN(y);
                pc += 2;
            }
            break;
        }
        case LXA: {
            if (subCycles == 1) {
                t = memory.read(pc + 1);
                x = (accumulator | magic) & t;
                accumulator = x;
                updateZN(x);
                pc += 2;
            }
            break;
        }
        case TAX: {
            if (subCycles == 1) {
                x = accumulator;
                updateZN(accumulator);
                pc++;
            }
            break;
        }
        case TAY: {
            if (subCycles == 1) {
                y = accumulator;
                updateZN(accumulator);
                pc++;
            }
            break;
        }
        case TSX: {
            if (subCycles == 1) {
                x = stack;
                updateZN(x);
                pc++;
            }
            break;
        }
        case TXA: {
            if (subCycles == 1) {
                accumulator = x;
                updateZN(accumulator);
                pc++;
            }
            break;
        }
        case TYA: {
            if (subCycles == 1) {
                accumulator = y;
                updateZN(accumulator);
                pc++;
            }
            break;
        }
        case TXS: {
            if (subCycles == 1) {
                stack = x;
                pc++;
            }
            break;
        }
        case EOR: {
            if (subCycles == 1) {
                accumulator ^= memory.read(pc + 1);
                updateZN(accumulator);
                pc += 2;
            }
        }
        case EOR_0X:
        case EOR_0: {
            if (subCycles == 1) {
                u8 mem = memory.read(pc + 1);
                if (instruction == EOR_0X) mem += x;
                accumulator ^= memory.read(mem);
                updateZN(accumulator);
                pc += 2;
            }
        }
        case ASL:
        case ROL: {
            if (subCycles == 1) {
                bool c = statusC;
                if (accumulator & 0x80) setStatusC;
                else clearStatusC;
                accumulator = accumulator << 1;
                if ((instruction == ROL) && c) accumulator |= 0x01;
                updateZN(accumulator);
                pc++;
            }
            break;
        }
        case LSR:
        case ROR: {
            if (subCycles == 1) {
                bool c = statusC;
                if (accumulator & 0x01) setStatusC;
                else clearStatusC;
                accumulator = accumulator >> 1;
                if ((instruction == ROR) && c) accumulator |= 0x80;
                updateZN(accumulator);
                pc++;
            }
            break;
        }
        case JMP_I:
        case JMP: {
            switch (subCycles) {
                case 2:
                    t = memory.read(pc + 1);
                    break;
                case 1:
                    if (instruction == JMP_I) pc += memory.read(pc + 2) << 8;
                    else pc = memory.read(pc + 2) << 8;
                    pc += t;
                    break;
            }
            break;
        }
        case JSR: {
            switch (subCycles) {
                case 4:
                    stackPush((pc + 2) >> 8);
                    break;
                case 3:
                    stackPush((pc + 2) & 0xff);
                    instructon = JMP; // size optimization
                    break;
            }
            break;
        }
        case RTS: {
            switch (subCycles) {
                case 3:
                    pc = stackPop();
                    break;
                case 2:
                    pc |= stackPop() << 8;
                    break;
                case 1:
                    pc++;
                    break;
            }
            break;
        }
        case SEI: {
            if (subCycles == 1) {
                setStatusI;
                pc++;
            }
            break;
        }
        case SEC: {
            if (subCycles == 1) {
                setStatusC;
                pc++;
            }
            break;
        }
        case SED: {
            if (subCycles == 1) {
                setStatusD;
                pc++;
            }
            break;
        }
        case CLI: {
            if (subCycles == 1) {
                clearStatusI;
                pc++;
            }
            break;
        }
        case CLC: {
            if (subCycles == 1) {
                clearStatusC;
                pc++;
            }
            break;
        }
        case CLD: {
            if (subCycles == 1) {
                clearStatusD;
                pc++;
            }
            break;
        }
        case CLV: {
            if (subCycles == 1) {
                clearStatusV;
                pc++;
            }
            break;
        }
        case DEX: {
            if (subCycles == 1) {
                x--;
                updateZN(x);
                pc++;
            }
            break;
        }
        case DEY: {
            if (subCycles == 1) {
                y--;
                updateZN(y);
                pc++;
            }
            break;
        }
        case INX: {
            if (subCycles == 1) {
                x++;
                updateZN(x);
                pc++;
            }
            break;
        }
        case INY: {
            if (subCycles == 1) {
                y++;
                updateZN(y);
                pc++;
            }
            break;
        }
        case PHA: {
            if (subCycles == 1) {
                stackPush(accumulator);
                pc++;
            }
            break;
        }
        case PHP: {
            if (subCycles == 1) {
                stackPush(status | 0b00110000);
                pc++;
            }
            break;
        }
        case PLA: {
            if (subCycles == 1) {
                accumulator = stackPop();
                pc++;
            }
            break;
        }
        case PLP: {
            if (subCycles == 1) {
                status = stackPop() | 0b00110000;
                pc++;
            }
            break;
        }
        case BRK: {
            switch (subCycles) {
                case 5:
                    stackPush((pc + 2) >> 8);
                    break;
                case 4:
                    stackPush((pc + 2) & 0xff);
                    break;
                case 3:
                    stackPush(status | 0b00110000);
                    break;
                case 2:
                    if (recievedNMI)
                        t = 0xfa;
                    else // IRQ or BRK
                        t = 0xfe;
                    pc = memory.read(0xff00 + t);
                    break;
                case 1:
                    pc = pc | (memory.read(0xff01 + t) << 8);
                    break;
                default:
                    break;
            }
            break;
        }
        case RTI: {
            switch (subCycles) {
                case 3:
                    status = stackPop() | 0b00110000;
                    break;
                case 2:
                    pc = stackPop();
                    break;
                case 1:
                    pc |= stackPop() << 8;
                    break;
                default:
                    break;
            }
            break;
        }
        case AND: {
            if (subCycles == 1) {
                accumulator &= memory.read(pc + 1);
                updateZN(accumulator);
                pc += 2;
            }
        }
        case ANC:
        case ANC_R: {
            if (subCycles == 1) {
                accumulator &= memory.read(pc + 1);
                updateZN(accumulator);
                if (statusN) setStatusC;
                else clearStatusC;
                pc += 2;
            }
        }
        case 0x02: // JAM
        case 0x12: // JAM
        case 0x22: // JAM
        case 0x32: // JAM
        case 0x42: // JAM
        case 0x52: // JAM
        case 0x62: // JAM
        case 0x72: // JAM
        case 0x92: // JAM
        case 0xB2: // JAM
        case 0xD2: // JAM
        case 0xF2: // JAM
            return; // Prevent the CPU from continuing
        case 0x0C: // 3-byte NOP
        case 0x1C: // 3-byte NOP
        case 0x3C: // 3-byte NOP
        case 0x5C: // 3-byte NOP
        case 0x7C: // 3-byte NOP
        case 0xDC: // 3-byte NOP
        case 0xFC: // 3-byte NOP
            if (subCycles == 1) pc += 3;
            break;
        case 0x04: // 2-byte NOP
        case 0x14: // 2-byte NOP
        case 0x34: // 2-byte NOP
        case 0x44: // 2-byte NOP
        case 0x54: // 2-byte NOP
        case 0x64: // 2-byte NOP
        case 0x74: // 2-byte NOP
        case 0x80: // 2-byte NOP
        case 0x82: // 2-byte NOP
        case 0x89: // 2-byte NOP
        case 0xC2: // 2-byte NOP
        case 0xD4: // 2-byte NOP
        case 0xE2: // 2-byte NOP
        case 0xF4: // 2-byte NOP
            if (subCycles == 1) pc += 2;
            break;
        case NOP:
        default: // Assumed 1-byte NOP
            if (subCycles == 1) pc++;
            break;
    }
    subCycles--;
}

void CPU::doWriteback() {
    if (writeback.needsWrite) {
        memory.write(writeback.location, writeback.data);
    }
    writeback.needsWrite = false;
}

void CPU::stackPush(u8 value) {
    writeback.needsWrite = true;
    writeback.location = 0x100 + stack;
    writeback.data = value;
    stack--;
}

u8 CPU::stackPop() {
    u8 value = memory.read(0x100 + stack);
    stack++;
    return value;
}

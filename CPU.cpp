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
// This bit of the status is unused
#define statusV (status & 0b01000000)
#define statusN (status & 0b10000000)

#define setStatusC status |= 0b00000001
#define setStatusZ status |= 0b00000010
#define setStatusI status |= 0b00000100
#define setStatusD status |= 0b00001000
#define setStatusB status |= 0b00010000
// This bit of the status is unused
#define setStatusV status |= 0b01000000
#define setStatusN status |= 0b10000000
#define clearStatusC status &= 0b11111110
#define clearStatusZ status &= 0b11111101
#define clearStatusI status &= 0b11111011
#define clearStatusD status &= 0b11110111
#define clearStatusB status &= 0b11101111
// This bit of the status is unused
#define clearStatusV status &= 0b10111111
#define clearStatusN status &= 0b01111111

#define sp stack

// ISA Reference: https://www.masswerk.at/6502/6502_instruction_set.html

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
    recievedIRQ = !statusI; // Interrupt Disable
    setStatusI;
}

void CPU::doCycle() {
    if (!subCycles || resetCPU) {
        if (resetCPU) resetCPU = false;
        else switch (instruction) {
                // TODO: increment program counter
            case BRK: // Interrupts
                resetCPU = false;
                recievedNMI = false;
                recievedIRQ = false;
                break;
            case LDA:
            case 0x04:
            case 0x14:
            case 0x34:
            case 0x44:
            case 0x54:
            case 0x64:
            case 0x74:
            case 0x80:
            case 0x82:
            case 0x89:
            case 0xC2:
            case 0xD4:
            case 0xE2:
            case 0xF4:
                pc++;
            case SEI:
            case SED:
            case SEC:
            case CLI:
            case CLD:
            case CLC:
            case CLV:
            case DEX:
            case DEY:
            case INX:
            case INY:
            case PHP:
            case PHA:
            case ROL_A:
            case ASL_A:
            default:
                pc++;
                break;
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
            case PHP:
            case PHA:
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
            case DEX:
            case DEY:
            case INX:
            case INY:
            case ROL_A:
            case ASL_A:
            case NOP:
            default:
                subCycles = 2;
                break;
        }
    }
    doInstruction();
}

void CPU::doInstruction() {
    switch (instruction) { // TODO: implement instructions
        case LDA:
            if (subCycles == 1) {
                accumulator = memory.read(pc + 1);
            }
            break;
        case ROL_A:
        case ASL_A:
            if (subCycles == 1) {
                bool c = statusC;
                if (accumulator & 0x80) setStatusC;
                else clearStatusC;
                accumulator = accumulator << 1;
                if ((instruction == ROL_A) && c) accumulator++;
                if (!accumulator) setStatusZ;
                else clearStatusZ;
            }
            break;
        case SEI:
            if (subCycles == 1) setStatusI;
            break;
        case SEC:
            if (subCycles == 1) setStatusC;
            break;
        case SED:
            if (subCycles == 1) setStatusD;
            break;
        case CLI:
            if (subCycles == 1) clearStatusI;
            break;
        case CLC:
            if (subCycles == 1) clearStatusC;
            break;
        case CLD:
            if (subCycles == 1) clearStatusD;
            break;
        case CLV:
            if (subCycles == 1) clearStatusV;
            break;
        case DEX:
            if (subCycles == 1) x--;
            break;
        case DEY:
            if (subCycles == 1) y--;
            break;
        case INX:
            if (subCycles == 1) x++;
            break;
        case INY:
            if (subCycles == 1) y++;
            break;
        case PHA:
            if (subCycles == 1) stackPush(accumulator);
            break;
        case PHP:
            if (subCycles == 1) stackPush(status | 0b00110000);
            break;
        case BRK:
            switch (subCycles) {
                case 5:
                    stackPush(pc >> 8);
                    break;
                case 4:
                    stackPush(pc & 0xff);
                    break;
                case 3:
                    stackPush(status);
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
        case NOP:
        default:
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
    stack++;
}

u8 CPU::stackPop() {
    u8 value = memory.read(0x100 + stack);
    stack--;
    return value;
}

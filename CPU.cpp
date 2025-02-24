//
//  CPU.cpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#include <arpa/inet.h> // ntohs

#include "Common.hpp"
#include "CPU.hpp"

#define read16(memory, address) ntohs(memory.read(memory) + (memory.read(memory + 1) << 8))

CPU::CPU(Memory &memory) : memory(memory) {
    accumulator = 0;
    x = 0;
    y = 0;
    stack = 0;
    status = 0;
}

void CPU::reset() {
    doingInterupt = true;
    interupt = RESET;
}

void CPU::transmitNMI() {
    if (doingInterupt) return;
    doingInterupt = true;
    interupt = NMI;
}

void CPU::transmitIRQ() {
    if (doingInterupt) return;
    doingInterupt = true;
    interupt = IRQ;
}

void CPU::doCycle() {
    if (!subCycles) {
        switch (instruction) {
                // TODO: increment program counter
            case 0:
                doingInterupt = false;
                interupt = IRQ; // For BRK instruction
                break;
            default:
                pc++;
                break;
        }
        if (doingInterupt) instruction = 0; // BRK
        else instruction = memory.read(pc);
        subCycles = 1;
        switch (instruction) {
                // TODO: set cycle count
            case 0:
                subCycles = 7;
                break;
            default:
                subCycles = 1;
                break;
        }
    }
    doInstruction();
}

void CPU::doInstruction() {
    switch (instruction) { // TODO: implement instructions
        case 0:
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
                    switch (interupt) {
                        case NMI:
                            t = 0xfa;
                            break;
                        case IRQ:
                            t = 0xfe;
                            break;
                        case RESET:
                            t = 0xfc;
                            break;
                    }
                    pc = memory.read(0xff00 + t);
                case 1:
                    pc = pc | (memory.read(0xff01 + t) << 8);
                default:
                    break;
            }
            break;
        default: // nop
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

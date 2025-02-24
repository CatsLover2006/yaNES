//
//  CPU.hpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#ifndef CPU_6502_hpp
#define CPU_6502_hpp

#include "Common.hpp"
#include "Memory.hpp"

struct writebackData {
    u16 location;
    u8 data;
    bool needsWrite;
};

enum interuptType {
    NMI,
    IRQ,
    RESET
};

class CPU {
private:
    Memory &memory;
    u8 accumulator, x, y, stack, status, t;
    u16 pc;
    writebackData writeback;
    bool doingInterupt;
    interuptType interupt;
    u8 subCycles, instruction;
    void doInstruction();
public:
    void doCycle();
    void doWriteback();
    void transmitNMI();
    void transmitIRQ();
    void reset();
    void stackPush(u8 value);
    u8 stackPop();
    CPU(Memory &memory);
};

#endif /* CPU_hpp */

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

class CPU {
private:
    Memory &memory;
    u8 t;
    writebackData writeback;
    bool dualInterrupt, recievedNMI, recievedIRQ, resetCPU;
    u8 subCycles, instruction;
    void doInstruction();
public:
    u8 accumulator, x, y, stack, status;
    u16 pc;
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

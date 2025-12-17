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

class CPU {
private:
    Memory &memory;
    u8 t;
    u16 t16;
    bool recievedInterrupt;
    u8 recievedNMI, recievedIRQ, resetCPU, instruction;
    s8 subCycle;
    void doInstruction();
public:
    u8 accumulator, x, y, stack, status;
    u16 pc;
    void doCycle();
    void setNMI();
    void clearNMI();
    void setIRQ();
    void clearIRQ();
    void setReset();
    void clearReset();
    void reset();
    void stackPush(u8 value);
    u8 stackPop();
    CPU(Memory &memory);
};

#endif /* CPU_hpp */

//
//  Test.cpp
//  
//
//  Created by Chance Parsons on 2025-02-24.
//

#include <criterion/criterion.h>
#include <iostream>

#include "Common.hpp"
#include "CPU.hpp"
#include "Memory.hpp"
#include "InstructionSet.h"

Test(LDA_immediate, test) {
    Memory memory;
    CPU cpu(memory);
    memory.write(0, LDA);
    memory.write(1, 69);
    memory.write(0xfffe, 0x10);
    memory.write(0xffff, 0x10);
    memory.write(0xfffc, 0);
    memory.write(0xfffd, 0);
    cpu.reset();
    // Do 2 cycles to finish LDA instruction
    for (int i = 0; i < 9; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 69);
}

Test(ROL_accumulator, test) {
    Memory memory;
    CPU cpu(memory);
    memory.write(0, LDA);
    memory.write(1, 1);
    // Write 13 ROLs
    for (int i = 2; i < 15; i++)
        memory.write(i, ROL);
    // Set up vectors
    memory.write(0xfffe, 0x10);
    memory.write(0xffff, 0x10);
    memory.write(0xfffc, 0);
    memory.write(0xfffd, 0);
    cpu.reset();
    // Do 2 cycles to finish test setup
    for (int i = 0; i < 9; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 1);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 2);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 4);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 8);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 16);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 32);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 64);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 128);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 0);
    cr_expect(cpu.status & 0b00000010, "Zero not set");
    cr_expect(cpu.status & 0b00000001, "Carry not set");
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 1);
    cr_expect(!(cpu.status & 0b00000010), "Zero set");
    cr_expect(!(cpu.status & 0b00000001), "Carry set");
}

Test(ASL_accumulator, test) {
    Memory memory;
    CPU cpu(memory);
    memory.write(0, 0xa9);
    memory.write(1, 1);
    // Write 13 ASLs
    for (int i = 2; i < 15; i++)
        memory.write(i, ASL);
    // Set up vectors
    memory.write(0xfffe, 0x10);
    memory.write(0xffff, 0x10);
    memory.write(0xfffc, 0);
    memory.write(0xfffd, 0);
    cpu.reset();
    // Do 2 cycles to finish test setup
    for (int i = 0; i < 9; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 1);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 2);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 4);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 8);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 16);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 32);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 64);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 128);
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 0);
    cr_expect(cpu.status & 0b00000010, "Zero not set");
    cr_expect(cpu.status & 0b00000001, "Carry not set");
    for (int i = 0; i < 2; i++)
        cpu.doCycle();
    cr_expect(cpu.accumulator == 0);
    cr_expect(cpu.status & 0b00000010, "Zero not set");
    cr_expect(!(cpu.status & 0b00000001), "Carry set");
}

Test(JAM, test) {
    Memory memory;
    CPU cpu(memory);
    memory.write(0xfffc, 0);
    memory.write(0xfffd, 0);
    u8 values[] = {0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x92, 0xB2, 0xD2, 0xF2};
    for (u8 value : values) {
        for (u16 i = 0; i < 0xff00; i++)
            memory.write(i, value);
        cpu.reset();
        // Do 1000 cycles
        for (int i = 0; i < 1000; i++)
            cpu.doCycle();
        cr_expect(cpu.pc == 1, "0x%x failed to jam", value);
    }
    memory.write(0, LDA);
    memory.write(1, 1);
    memory.write(2, ROL);
    cpu.reset();
    for (int i = 0; i < 11; i++) cpu.doCycle();
    cr_assert(cpu.pc, "CPU failed to unjam after reset");
    cr_expect(cpu.accumulator == 2, "CPU does not run code properly after jam and reset");
} //*///



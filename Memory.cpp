//
//  Memory.cpp
//  
//
//  Created by Chance Parsons on 2025-02-07.
//

#include <cstdlib>

#include "Common.hpp"
#include "Memory.hpp"

Memory::Memory() {
    memory = (u8*) malloc(0xffff);
}

Memory::Memory(u8* memory) : memory(memory) {};

Memory::~Memory() {
    free(memory);
}

u8 Memory::read(u16 location) {
    return memory[location];
}

void Memory::write(u16 location, u8 value) {
    memory[location] = value;
}

RAM::RAM() {
    ram = (u8*) malloc(0x800);
}

RAM::~RAM() {
    free(ram);
}

u8 RAM::read(u16 location) {
    return ram[location & 0x7ff];
}

void RAM::write(u16 location, u8 value) {
    ram[location & 0x7ff] = value;
}

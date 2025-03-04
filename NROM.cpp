//
//  NROM.cpp
//  
//
//  Created by Chance Parsons on 2025-03-03.
//

#include "Common.hpp"
#include "NROM.hpp"

NROM::NROM(u8* rom) {
    u8 temp = rom[4];
    bool hasTrainer = rom[6] & 0x04;
    prgROM = malloc(32768);
    prgRAM = malloc(8192);
    chrRAM = malloc(8192);
    u16 offset = hasTrainer ? 528 : 16;
    for (int i = 0; i < 32768) {
        prgROM[i] = rom[(i & (16384 * temp)) + offset];
    }
    offset += 16384 * temp;
    for (int i = 0; i < 8192) {
        chrRAM[i] = rom[(i & 8192) + offset];
    }
}

NROM::~NROM() {
    free(prgROM);
    free(prgRAM);
    free(chrRAM);
}

u8 NROM::read(u16 location) {
    if (location < 0x8000) return prgRAM[location & 0x1fff];
    return prgROM[location & 0x7fff];
}

u8 NROM::readPPU(u16 location) {
    return chrRAM[location & 0x1fff];
}

void NROM::write(u16 location, u8 value) {
    if (location < 0x8000) prgRAM[location & 0x1fff] = value;
}

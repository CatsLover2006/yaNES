//
//  NROM.cpp
//  
//
//  Created by Chance Parsons on 2025-03-03.
//


#include <cstdlib>

#include "../Common.hpp"
#include "NROM.hpp"

/**
 * Supported iNES Mappers:
 * - Mapper 0 (no PRG RAM)
 */

NROM::NROM(u8* prgROM, u8 prgSize, u8* chrROM, u8 nametableMirroring) {
    this->prgMask = (prgSize == 2) ? 0x7fff : 0x3fff;
    this->nametableMirroring = nametableMirroring;
    this->prgROM = (u8*) malloc(16384 * prgSize);
    this->chrROM = (u8*) malloc(8192);
    for (int i = 0; i < 16384 * prgSize; i++)
        this->prgROM[i] = prgROM[i];
    for (int i = 0; i < 8192; i++)
        this->chrROM[i] = chrROM[i];
}

NROM::~NROM() {
    free(prgROM);
    free(chrROM);
}

u8 NROM::read(u16 location) {
    return prgROM[location & prgMask];
}

u8 NROM::readPPU(u16 location) {
    return chrROM[location & 0x1fff];
}

void NROM::writePPU(u16 location, u8 value) { }

void NROM::write(u16 location, u8 value) { }

u8 NROM::currentNametableMirroring() {
    return nametableMirroring;
}


//
//  PPU.cpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#include <cstdlib>

#include "Common.hpp"
#include "PPU.hpp"

PPU::PPU() {
    oam = (u8*) malloc(0x100);
    for (int i = 0; i < 0x100; i++) oam[i] = 0;
};

PPU::~PPU() {
    free(oam);
};

void PPU::attachCart(Cartridge * cart) {
    cartridge = cart;
};

void PPU::writeOAM(u8 location, u8 value) {
    oam[location] = value;
};

void PPU::doCycle() {
    // TODO: PPU
};

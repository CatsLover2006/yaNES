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
    ciram = (u8*) malloc(0x800);
    palette = (u8*) malloc(0x20);
    reset();
};

PPU::~PPU() {
    free(oam);
};

void PPU::reset() {
    for (int i = 0; i < 0x100; i++) oam[i] = 0;
    ppuscroll = false;
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

void PPU::write(u16 location, u8 value) {
    if (location & 0x8000) // Registers
        switch (location) {
            case 0x8000:
                ppuctrl = value;
                return;
            case 0x8001:
                ppumask = value;
                return;
            case 0x8002:
                ppustatus = value;
                return;
            case 0x8003:
                if (ppuscroll) ppuscrollx = value;
                else ppuscrolly = value;
                ppuscroll = !ppuscroll;
                return;
            default:
                return;
        }
    if (location >= 0x3f00) {
        palette[location & 0x20] = value;
        return;
    }
    if (location >= 0x2000) {
        // TODO: nametables
        return;
    }
    cartridge->writePPU(location, value);
}

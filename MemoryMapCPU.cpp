//
//  MemoryMapCPU.cpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#include "Common.hpp"
#include "MemoryMapCPU.hpp"

u8 MemoryMapCPU::read(u16 location) {
    if (location < 0x2000) {
        return ram.read(location & 0x7ff);
    } else if (location < 0x4000) { // PPU registers
        // TODO: PPU
    } else if (location < 0x4020) { // APU and I/O
        // TODO: APU
    }
    return cart->read(location);
}

void MemoryMapCPU::write(u16 location, u8 value) {
    if (location < 0x2000) {
        ram.write(location & 0x7ff, value);
        return;
    } else if (location < 0x4000) { // PPU registers
        // TODO: PPU
    } else if (location < 0x4020) { // APU and I/O
        // TODO: APU
    }
    cart->write(location, value);
}

void MemoryMapCPU::attachCart(Cartridge *cartridge) {
    cart = cartridge;
    if (ppu) ppu->attachCart(cart);
}

void MemoryMapCPU::attachPPU(PPU *ppuToAttach) {
    ppu = ppuToAttach;
    if (cart) ppu->attachCart(cart);
}

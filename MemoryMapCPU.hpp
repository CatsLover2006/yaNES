//
//  MemoryMapCPU.hpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#ifndef NESemu_MemoryMapCPU_hpp
#define NESemu_MemoryMapCPU_hpp

#include "Common.hpp"
#include "Memory.hpp"
#include "Cartridge.hpp"
#include "PPU.hpp"
#include "CPU.hpp"

class MemoryMapCPU : Memory {
private:
    PPU &ppu;
    Cartridge &cart;
    RAM ram;
public:
    u8 read(u16 location);
    void write(u16 location, u8 value);
    void attachCart(Cartridge &cartridge);
    void attachPPU(PPU &ppuToAttach);
    MemoryMapCPU();
};

#endif /* MemoryMapCPU_hpp */

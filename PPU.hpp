//
//  PPU.hpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#ifndef NESemu_PPU_hpp
#define NESemu_PPU_hpp

#include "Common.hpp"
#include "Cartridge.hpp"

struct Color {
    u8 r;
    u8 g;
    u8 b;
};

struct Frame {
    Color pixels[256][240];
};

class PPU {
private:
    Cartridge * cartridge;
    u8* oam;
    u8* ciram;
    u8* palette;
    bool ppuscroll;
    u8 ppuctrl, ppumask, ppustatus, ppuscrollx, ppuscrolly;
public:
    void attachCart(Cartridge * cart);
    void writeOAM(u8 location, u8 value);
    void write(u16 location, u8 value);
    void doCycle();
    PPU();
    ~PPU();
    Frame drawnFrame;
    void reset();
};

#endif /* PPU_hpp */

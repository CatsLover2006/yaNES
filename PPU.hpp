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
public:
    void attachCart(Cartridge * cart);
    void writeOAM(u8 location, u8 value);
    void doCycle();
    PPU();
    ~PPU();
    Frame drawnFrame;
};

#endif /* PPU_hpp */

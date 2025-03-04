//
//  NROM.hpp
//  
//
//  Created by Chance Parsons on 2025-03-03.
//

#ifndef NESemu_NROM_hpp
#define NESemu_NROM_hpp

#include "Common.hpp"
#include "Cartridge.hpp"

class NROM : Cartridge {
    u8* prgROM;
    u8* prgRAM;
    u8* chrRAM;
public:
    u8 read(u16 location);
    void write(u16 location, u8 value);
    u8 readPPU(u16 location);
    NROM(u8* rom);
    ~NROM();
};

#endif /* NROM_hpp */

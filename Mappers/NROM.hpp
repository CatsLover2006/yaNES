//
//  NROM.hpp
//  
//
//  Created by Chance Parsons on 2025-03-03.
//

#ifndef NESemu_NROM_hpp
#define NESemu_NROM_hpp

#include "../Common.hpp"
#include "../Cartridge.hpp"

class NROM : public Cartridge {
    u8* prgROM;
    u8* chrROM;
    u8 nametableMirroring;
    u16 prgMask;
public:
    u8 read(u16 location);
    void write(u16 location, u8 value);
    u8 readPPU(u16 location);
    void writePPU(u16 location, u8 value);
    u8 currentNametableMirroring();
    NROM(u8* prgROM, u8 prgSize, u8* chrROM, u8 nametableMirroring);
    ~NROM();
};

#endif /* NROM_hpp */

//
//  Cartridge.hpp
//  
//
//  Created by Half-Qilin on 2025-02-14.
//

#ifndef NESemu_Cartridge_hpp
#define NESemu_Cartridge_hpp

#include "Common.hpp"
#include "Memory.hpp"

class Cartridge : Memory {
public:
    virtual u8 read(u16 location);
    virtual void write(u16 location, u8 value);
    virtual u8 readPPU(u16 location);
};

#endif /* Cartridge_hpp */

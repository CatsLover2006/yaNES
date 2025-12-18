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
#include "CPU.hpp"

class Cartridge : public Memory {
public:
    virtual u8 read(u16 location);
    virtual void write(u16 location, u8 value);
    virtual u8 readPPU(u16 location);
    virtual void writePPU(u16 location, u8 value);
    virtual void attachCPU(CPU*);
    virtual void transmitM2();
    virtual u8* getSave();
    virtual void setSave(u8* save);
    virtual u8 currentNametableMirroring();
    Cartridge();
};

#endif /* Cartridge_hpp */

//
//  Memory.hpp
//  
//
//  Created by Chance Parsons on 2025-02-07.
//

#ifndef NESemu_Memory_hpp
#define NESemu_Memory_hpp

#include "Common.hpp"

class Memory {
public:
    virtual u8 read(u16 location);
    virtual void write(u16 location, u8 value);
};

class RAM : Memory {
private:
    byte* ram;
public:
    u8 read(u8 location);
    void write(u16 location, u8 value);
    RAM();
};

class NROM : Memory {
private:
    byte* rom;
public:
    u8 read(u8 location);
    void write(u16 location, u8 value);
    NROM(byte* rom);
};

#endif /* NESemu_Memory_hpp */

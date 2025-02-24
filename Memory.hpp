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
    u8* memory;
public:
    virtual u8 read(u16 location);
    virtual void write(u16 location, u8 value);
    Memory();
    ~Memory();
};

class RAM : Memory {
private:
    u8* ram;
public:
    u8 read(u16 location);
    void write(u16 location, u8 value);
    RAM();
    ~RAM();
};

#endif /* NESemu_Memory_hpp */

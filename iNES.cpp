//
//  iNES.cpp
//
//
//  Created by Half-Qilin on 2025-12-17.
//

#include "Common.hpp"
#include "iNES.hpp"
#include "Cartridge.hpp"
#include "Mappers/NROM.hpp"

Cartridge* loadiNES(u8* rom) {
    bool hasTrainer = rom[6] & 0x04;
    u16 offset = hasTrainer ? 528 : 16;
    u8 mapper = ((rom[6] >> 4) & 0x0f) | ((rom[7] << 4) & 0xf0);
    switch (mapper) {
        case 0:
            return new NROM(rom + offset, rom[4], rom + offset + rom[4] * 16384, rom[6] & 1);
    }
    return nullptr;
}

Cartridge* loadNES2(u8* rom) {
    // TODO: NES 2.0
}

Cartridge* loadNES(u8* rom) {
    if (!rom) return nullptr;
    u8 iNESheader[] = { 'N', 'E', 'S', 0x1A };
    if (!(*((u32*)rom) == *((u32*)iNESheader)))
        return nullptr; // Not an iNES ROM
    if ((rom[7] & 0x0C) == 0x08) return loadNES2(rom);
    else return loadiNES(rom);
}

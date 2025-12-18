//
//  iNES.hpp
//
//
//  Created by Half-Qilin on 2025-12-17.
//

#ifndef NESemu_iNES_hpp
#define NESemu_iNES_hpp

#include "Common.hpp"
#include "Cartridge.hpp"

Cartridge* loadNES(u8* rom);
Cartridge* loadiNES(u8* rom);
Cartridge* loadNES2(u8* rom);

#endif /* iNES_hpp */

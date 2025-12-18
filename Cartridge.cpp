//
//  Cartridge.cpp
//
//
//  Created by Half-Qilin on 2025-03-03.
//

#include "Cartridge.hpp"

Cartridge::Cartridge() {};

u8 Cartridge::read(u16 location) { return location & 0xff; };
u8 Cartridge::readPPU(u16 location) { return location & 0xff; };
u8* Cartridge::getSave() { return nullptr; }
void Cartridge::write(u16 location, u8 value) {};
void Cartridge::writePPU(u16 location, u8 value) {};
void Cartridge::attachCPU(CPU*) {};
void Cartridge::transmitM2() {};
void Cartridge::setSave(u8* save) {};
u8 Cartridge::currentNametableMirroring() { return 0; };

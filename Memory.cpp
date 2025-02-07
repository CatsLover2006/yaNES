//
//  Memory.cpp
//  
//
//  Created by Chance Parsons on 2025-02-07.
//

#include <cstdlib>

#include "Memory.hpp"

RAM::RAM() {
    ram = malloc(0x800);
}

RAM::~RAM() {
    free(ram);
}

Mapper0::Mapper0() {
    
}

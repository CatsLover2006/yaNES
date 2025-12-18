//
//  RunROM.cpp
//  
//
//  Created by Chance Parsons on 2025-12-17.
//

#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <filesystem>

#include "Common.hpp"
#include "CPU.hpp"
#include "iNES.hpp"
#include "MemoryMapCPU.hpp"

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "Input a ROM!" << std::endl;
        return 0;
    }
    std::filesystem::path p{argv[1]};
    std::ifstream infile(argv[1], std::ios_base::binary);
    u8* rom = (u8*)malloc(std::filesystem::file_size(p));
    infile.read((char*)rom, std::filesystem::file_size(p));
    infile.close();
    MemoryMapCPU memory;
    memory.attachCart(loadNES(rom));
    CPU cpu(memory);
    cpu.reset();
    while (true) cpu.doCycle();
    return 0;
}

clangpp="$(which clang++)"
if [[ -z "${clangpp}" ]]; then
    clangpp="$(which g++)"
fi
if [[ -z "${clangpp}" ]]; then
    clangpp="$(which c++)"
fi
if [[ -z "${clangpp}" ]]; then
    echo "Could not find C++ compiler; we can't continue!"
else
    echo "C++ compiler: ${clangpp}"
    ${clangpp} --std=c++17 Cartridge.cpp CPU.cpp Memory.cpp MemoryMapCPU.cpp PPU.cpp RunROM.cpp iNES.cpp */*.cpp -o RunROM && echo "Running ROM..." && ./RunROM $1
fi

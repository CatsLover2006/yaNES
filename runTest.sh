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
    ${clangpp} -lcriterion --std=c++14 *.cpp */*.cpp -o Test && echo "Running tests..." && ./Test
fi

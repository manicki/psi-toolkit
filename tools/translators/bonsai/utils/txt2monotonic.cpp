#include <string>
#include <iostream>
#include <cstdio>
#include <inttypes.h>

#include "MonotonicVector.hpp"

int main(int argc, char** args) {
    poleng::bonsai::MonotonicVector<unsigned int, unsigned int, 32> mv;
    
    uint64_t pos;
    while(std::cin >> pos)
        mv.push_back(pos);
    mv.commit();
    
    if(argc > 1) {
        std::FILE* out = std::fopen(args[1], "w");   
        mv.save(out);
        std::fclose(out);
    }
    
    return 0;
}
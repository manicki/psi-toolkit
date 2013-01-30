#include <string>
#include <iostream>
#include <cstdio>

#include "StringVector.hpp"

int main(int argc, char** args) {
    StringVector<> sv;
    
    std::string line;
    while(std::cin >> line)
        sv.push_back(line);
    
    if(argc > 0) {
        std::FILE* out = std::fopen(args[1], "w");   
        sv.save(out);
        std::fclose(out);
    }
    
    return 0;
}
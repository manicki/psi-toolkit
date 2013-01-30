#include <string>
#include <iostream>
#include <sstream>

#include "StringVector.hpp"

int main(int argc, char** argv) {
    poleng::bonsai::StringVector<> sv;
    sv.load(argv[1]);
    
    size_t pos;
    while(std::cin >> pos) {
        std::stringstream ss(sv[pos]);
        
        unsigned char l, m, n;
        unsigned int j;
        while(ss >> l >> m >> n) {
            std::cout << (int)l << " " << (int)m << " " << (int)n << " "; 
            for(int i = 0; i < l; i++) {
                ss.read((char*)&j, 4);
                std::cout << j << " ";
            }
            for(int i = 0; i < m; i++) {
                ss.read((char*)&j, 4);
                std::cout << j << " ";
            }
            for(int i = 0; i < n; i++) {
                ss.read((char*)&j, 4);
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }
}
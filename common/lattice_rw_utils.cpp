#include <iomanip>
#include <iostream>

#include "lattice_rw_utils.hpp"


std::string LatticeRWUtils::escape(std::string formatName, std::string str) {
    std::string result = "";
    for (int i = 0; i < str.length(); ++i) {
        switch (str[i]) {
            case ' ' : result += "_"; break;
            default : result += str[i];
        }
    }
    return result;
}

std::string LatticeRWUtils::unescape(std::string formatName, std::string str) {
    std::string result = "";
    for (int i = 0; i < str.length(); ++i) {
        switch (str[i]) {
            case '_' : result += " "; break;
            default : result += str[i];
        }
    }
    return result;
}

int LatticeRWUtils::utf8SymbolLength(unsigned char firstByte) {
    if ((firstByte & 0200) == 0000) return 1;
    if ((firstByte & 0340) == 0300) return 2;
    if ((firstByte & 0360) == 0340) return 3;
    if ((firstByte & 0370) == 0360) return 4;
    if ((firstByte & 0374) == 0370) return 5;
    if ((firstByte & 0376) == 0374) return 6;
}

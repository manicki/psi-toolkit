#ifndef LATTICE_RW_UTILS_HDR
#define LATTICE_RW_UTILS_HDR


#include <string>


namespace LatticeRWUtils {
    
    std::string escape(std::string formatName, std::string str);

    std::string unescape(std::string formatName, std::string str);
    
    /**
     * returns the number of bytes in the UTF-8 symbol depending on the first byte
     */
    int utf8SymbolLength(unsigned char firstByte);

}


#endif

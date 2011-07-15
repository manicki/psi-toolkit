#ifndef LATTICE_RW_UTILS_HDR
#define LATTICE_RW_UTILS_HDR


#include <string>
#include <vector>


namespace LatticeRWUtils {
    
    std::string escape(std::string formatName, std::string str);

    std::string unescape(std::string formatName, std::string str);
    
}


class UTF8String : public std::string {
    
public:
    
    typedef std::vector<std::string>::iterator iterator;
    
    UTF8String(std::string str); 
    
    int length() { return s_.size(); }
    
    iterator begin() { return s_.begin(); }
    iterator end() { return s_.end(); }

private:

    std::vector<std::string> s_;
    
    /**
     * returns the number of bytes in the UTF-8 symbol depending on the first byte
     */
    int utf8SymbolLength_(unsigned char firstByte);
    
};


#endif

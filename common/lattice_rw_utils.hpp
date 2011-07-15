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
    typedef std::vector<std::string>::reverse_iterator reverse_iterator;
    
    UTF8String(std::string str); 
    
    iterator begin() { return s_.begin(); }
    iterator end() { return s_.end(); }
    reverse_iterator rbegin() { return s_.rbegin(); }
    reverse_iterator rend() { return s_.rend(); }

    size_t length() { return s_.size(); }
    size_t max_size() { return s_.max_size(); }
    
    const std::string& operator[](size_t pos) const { return s_[pos]; }
    std::string& operator[](size_t pos) { return s_[pos]; }
    const std::string& at(size_t pos) const { return s_.at(pos); }
    std::string& at(size_t pos) { return s_.at(pos); }
    
    std::string substr(size_t pos = 0, size_t n = npos) const;
    
private:

    std::vector<std::string> s_;
    
    /**
     * returns the number of bytes in the UTF-8 symbol depending on the first byte
     */
    int utf8SymbolLength_(unsigned char firstByte);
    
};


#endif

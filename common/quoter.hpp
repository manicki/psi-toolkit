#ifndef QUOTER_HDR
#define QUOTER_HDR

#include <string>


class Quoter {

public:

    /**
     * Escapes all special symbols.
     */
    std::string escape(std::string str);

    /**
     * Unescapes all escaped special symbols.
     */
    std::string unescape(std::string str);

private:

    virtual std::string doEscape(std::string str) = 0;
    virtual std::string doUnescape(std::string str) = 0;

};


#endif

#ifndef BRACKETING_QUOTER_HDR
#define BRACKETING_QUOTER_HDR


#include "quoter.hpp"


/**
 * Escaped characters:
 * new line -> '\n'
 * tabulation -> '\t'
 * carriage return -> '\r'
 * '\' -> '\\'
 */

class BracketingQuoter : public Quoter {

public:

    BracketingQuoter() { }
    virtual ~BracketingQuoter() { }

private:

    virtual std::string doEscape(std::string str);
    virtual std::string doUnescape(std::string str);

};


#endif

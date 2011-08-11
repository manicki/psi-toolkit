#ifndef UTT_LIKE_QUOTER_HDR
#define UTT_LIKE_QUOTER_HDR


#include "exceptions.hpp"
#include "quoter.hpp"


/**
 * Escaped characters:
 * ' ' -> '_'
 * new line -> '\n'
 * tabulation -> '\t'
 * carriage return -> '\r'
 * '_' -> '\_'
 * '*' -> '\*'
 * '\' -> '\\'
 */

class UTTLikeQuoter : public Quoter {

public:

    UTTLikeQuoter() { }

    UTTLikeQuoter(std::string addQuot) : additionalQuotations_(addQuot) { }

    virtual ~UTTLikeQuoter();

private:

    std::string additionalQuotations_;

    virtual std::string doEscape(std::string str);
    virtual std::string doUnescape(std::string str);

};


#endif

#ifndef UTT_LIKE_QUOTER_HDR
#define UTT_LIKE_QUOTER_HDR


#include <vector>

#include "quoter.hpp"


/**
 * Escaped characters:
 * ' ' -> '_'
 * '\' -> '\\'
 * '_' -> '\_'
 * new line -> '\n'
 */

class UTTLikeQuoter : public Quoter {

public:

    UTTLikeQuoter() { }

private:

    virtual std::string doEscape(std::string str);
    virtual std::string doUnescape(std::string str);

};


#endif

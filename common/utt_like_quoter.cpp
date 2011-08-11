#include "utt_like_quoter.hpp"

#include <iostream>

std::string UTTLikeQuoter::doEscape(std::string str) {
    std::string result = "";
    for (size_t i = 0; i < str.length(); ++i) {
        switch (str[i]) {
        case ' ' : result += "_"; break;
        case '\n' : result += "\\n"; break;
        case '\t' : result += "\\t"; break;
        case '\r' : result += "\\r"; break;
        case '_' : result += "\\_"; break;
        case '*' : result += "\\*"; break;
        case '\\' : result += "\\\\"; break;
        default :
            size_t j = additionalQuotations_.length();
            while (j) {
                if (str[i] == additionalQuotations_[j-1]) {
                    result += "\\";
                    result += str[i];
                    break;
                }
                --j;
            }
            if (j) break;
            result += str[i]; break;
        }
    }
    return result;
}

std::string UTTLikeQuoter::doUnescape(std::string str) {
    std::string result = "";
    for (size_t i = 0; i < str.length(); ++i) {
        switch (str[i]) {
        case '_' : result += " "; break;
        case '\\' :
            ++i;
            if (i < str.length()) {
                switch (str[i]) {
                case 'n' : result += "\n"; break;
                case 't' : result += "\t"; break;
                case 'r' : result += "\r"; break;
                case '_' : result += "_"; break;
                case '*' : result += "*"; break;
                case '\\' : result += "\\"; break;
                default :
                    size_t j = additionalQuotations_.length();
                    while (j) {
                        if (str[i] == additionalQuotations_[j-1]) {
                            result += str[i]; break;
                        }
                        --j;
                    }
                    if (j) break;
                    throw QuoterException("Wrong escape sequence.");
                    break;
                }
            } else {
                throw QuoterException("Wrong escape sequence.");
            }
            break;
        default : result += str[i];
        }
    }
    return result;
}

UTTLikeQuoter::~UTTLikeQuoter() {
}

#include "bracketing_quoter.hpp"

#include <iostream>

#include "exceptions.hpp"


std::string BracketingQuoter::doEscape(std::string str) {
    std::string result = "";
    for (size_t i = 0; i < str.length(); ++i) {
        switch (str[i]) {
        case '\n' : result += "\\n"; break;
        case '\t' : result += "\\t"; break;
        case '\r' : result += "\\r"; break;
        case '\\' : result += "\\\\"; break;
        default: result += str[i]; break;
        }
    }
    return result;
}


std::string BracketingQuoter::doUnescape(std::string str) {
    std::string result = "";
    for (size_t i = 0; i < str.length(); ++i) {
        switch (str[i]) {
        case '\\' :
            ++i;
            if (i < str.length()) {
                switch (str[i]) {
                case 'n' : result += "\n"; break;
                case 't' : result += "\t"; break;
                case 'r' : result += "\r"; break;
                case '\\' : result += "\\"; break;
                default: break;
                }
            } else {
                throw QuoterException("Wrong escape sequence.");
            }
            break;
        default: result += str[i];
        }
    }
    return result;
}

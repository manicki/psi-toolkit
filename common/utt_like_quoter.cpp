#include "utt_like_quoter.hpp"


std::string UTTLikeQuoter::doEscape(std::string str) {
    std::string result = "";
    for (int i = 0; i < str.length(); ++i) {
        switch (str[i]) {
        case ' ' : result += "_"; break;
        case '\\' : result += "\\\\"; break;
        case '_' : result += "\\_"; break;
        case '\n' : result += "\\n"; break;
        default : result += str[i];
        }
    }
    return result;
}

std::string UTTLikeQuoter::doUnescape(std::string str) {
    std::string result = "";
    for (int i = 0; i < str.length(); ++i) {
        switch (str[i]) {
        case '_' : result += " "; break;
        case '\\' :
            ++i;
            if (i < str.length()) {
                switch (str[i]) {
                case '\\' : result += "\\"; break;
                case '_' : result += "_"; break;
                case 'n' : result += "\n"; break;
                default : result += ("\\" + str[i]); break;
                }
            } else {
                result += "\\";
            }
            break;
        default : result += str[i];
        }
    }
    return result;
}


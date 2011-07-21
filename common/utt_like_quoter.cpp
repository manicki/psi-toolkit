#include "utt_like_quoter.hpp"


std::string UTTLikeQuoter::doEscape(std::string str) {
    std::string result = "";
    for (int i = 0; i < str.length(); ++i) {
        switch (str[i]) {
        case ' ' : result += "_"; break;
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
        default : result += str[i];
        }
    }
    return result;
}


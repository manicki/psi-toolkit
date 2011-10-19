
#include "puddle_util.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            namespace util {

                /**
                 * escapes special characters from string `s`
                 * original string remains unchanged
                 */
                std::string escapeSpecialChars(std::string &s) {
                    std::string r = s;

                    Pattern regAmp("&");
                    Pattern regLt("<");
                    Pattern regGt(">");
                    Pattern regLPar("\\(");
                    Pattern regRPar("\\)");
                    Pattern regAlt("\\|");
                    Pattern regPlus("\\+");
                    Pattern regAsterisk("\\*");
                    Pattern regOpt("\\?");

                    RegExp::GlobalReplace(&r, regAmp, "&amp;");
                    RegExp::GlobalReplace(&r, regLt, "&lt;");
                    RegExp::GlobalReplace(&r, regGt, "&gt;");
                    RegExp::GlobalReplace(&r, regLPar, "&#40;");
                    RegExp::GlobalReplace(&r, regRPar, "&#41;");
                    RegExp::GlobalReplace(&r, regAlt, "&#124;");
                    RegExp::GlobalReplace(&r, regPlus, "&#42;");
                    RegExp::GlobalReplace(&r, regAsterisk, "&#43;");
                    RegExp::GlobalReplace(&r, regOpt, "&#63;");

                    return r;
                }

                /**
                 * unescapes special characters from string `s`
                 * original string remains unchanged
                 */
                std::string unescapeSpecialChars(std::string &s) {
                    std::string r = s;
                    size_t pos = 0;

                    //left parenthesis
                    pos = r.find("&#40;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "(" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#40;", pos + 1);
                    }
                    //right parenthesis
                    pos = r.find("&#41;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + ")" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#41;", pos + 1);
                    }
                    //asterisk
                    pos = r.find("&#42;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "*" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#42;", pos + 1);
                    }
                    //plus sign
                    pos = r.find("&#43;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "+" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#43;", pos + 1);
                    }
                    //ampersand
                    pos = r.find("&amp;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "&" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&amp;", pos + 1);
                    }
                    //less-than sign
                    pos = r.find("&lt;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "<" + r.substr(pos + 4, std::string::npos);
                        pos = r.find("&lt;", pos + 1);
                    }
                    //greater-than sign
                    pos = r.find("&gt;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + ">" + r.substr(pos + 4, std::string::npos);
                        pos = r.find("&gt;", pos + 1);
                    }
                    //question mark
                    pos = r.find("&#63;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "?" + r.substr(pos + 5, std::string::npos);
                        pos = r.find("&#63;", pos + 1);
                    }
                    //vertical bar
                    pos = r.find("&#124;");
                    while (pos != std::string::npos) {
                        r = r.substr(0, pos) + "|" + r.substr(pos + 6, std::string::npos);
                        pos = r.find("&#124;", pos + 1);
                    }

                    return r;
                }

                std::string getMorphologyString(Morphology morphology) {
                    std::string s = "";
                    std::string partOfSpeech = morphology["pos"];
                    s = partOfSpeech;
                    for (Morphology::iterator morphoIt = morphology.begin();
                            morphoIt != morphology.end(); ++ morphoIt) {
                        std::string attribute = morphoIt->first;
                        std::string value = morphoIt->second;
                        if (attribute == "pos")
                            continue;
                        if (s != "")
                            s += ":";
                        s += value;
                    }
                    return s;
                }

                bool getGroupActionParams(RuleTokenSizes &ruleTokenSizes,
                    unsigned int start, unsigned int head, unsigned int end,
                    int &realStart, int &realHead, int &realEnd) {
                    realStart = 0;
                    realHead = 0;
                    realEnd = 0;
                    size_t i = 0;
                    while (i < ruleTokenSizes.size()) {
                        if (i < start)
                            realStart += ruleTokenSizes[i];
                        if (i >= start) {
                            if (i < (head - 1)) {
                                realHead += ruleTokenSizes[i];
                            }
                        }
                        if (i == (head - 1)) {
                            if (ruleTokenSizes[i] == 0) {
                                return false;
                            }
                        }
                        if (i <= end)
                            realEnd += ruleTokenSizes[i];
                        else
                            break;
                        i ++;
                    }
                    realEnd --;

                    if (realStart > realEnd)
                        return false;

                    return true;
                }


                bool getSyntokActionParams(RuleTokenSizes &ruleTokenSizes,
                        unsigned int start, unsigned int end,
                        int &realStart, int &realEnd) {
                    realStart = 0;
                    realEnd = 0;
                    size_t i = 0;
                    while (i < ruleTokenSizes.size()) {
                        if (i < start)
                            realStart += ruleTokenSizes[i];
                        if (i <= end)
                            realEnd += ruleTokenSizes[i];
                        else
                            break;
                        i ++;
                    }
                    realEnd --;
                    return true;
                }

                int getDeleteActionParams(RuleTokenSizes &ruleTokenSizes,
                        int tokenIndex) {
                    int before = 0;
                    int i = 0;
                    while (i < (tokenIndex - 1)) {
                        before += ruleTokenSizes[i];
                        i ++;
                    }
                    return before;
                }

                int getAddActionParams(RuleTokenSizes &ruleTokenSizes,
                        int tokenIndex) {
                    int before = 0;
                    int i = 0;
                    while (i < (tokenIndex - 1)) {
                        before += ruleTokenSizes[i];
                        i ++;
                    }
                    return before;
                }

                int getTransformActionParams(RuleTokenSizes &ruleTokenSizes,
                        int element) {
                    int before = 0;
                    int i = 0;
                    while (i < (element - 1)) {
                        before += ruleTokenSizes[i];
                        i ++;
                    }
                    return before;
                }

            }
        }

    }

}


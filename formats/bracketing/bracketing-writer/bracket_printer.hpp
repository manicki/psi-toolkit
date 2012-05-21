#ifndef BRACKET_PRINTER_HDR
#define BRACKET_PRINTER_HDR


#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include "edge_data.hpp"


template<int N>
class BracketPrinter {

public:

    BracketPrinter(
        std::string patterns[N],
        std::string tagSeparator,
        std::string avPairsSeparator,
        std::string avSeparator
    ) :
        patterns_(patterns),
        tagSeparator_(tagSeparator),
        avPairsSeparator_(avPairsSeparator),
        avSeparator_(avSeparator)
    { }

    std::set<std::string[N]> print(
        std::set<EdgeData> edgeDataSet
    );

private:

    std::string patterns_[N];
    std::string tagSeparator_;
    std::string avPairsSeparator_;
    std::string avSeparator_;

    char close_(char c) {
        switch (c) {
        case '(' : return ')';
        case ')' : return '(';
        case '[' : return ']';
        case ']' : return '[';
        case '{' : return '}';
        case '}' : return '{';
        case '<' : return '>';
        case '>' : return '<';
        default : return c;
    }
}

};


template<int N>
std::set<std::string[N]> BracketPrinter<N>::print(
    std::set<EdgeData> edgeDataSet
) {
    std::set<std::string[N]> result;
    BOOST_FOREACH(EdgeData edgeData, edgeDataSet) {
        std::string resultElement[N];
        for (int I = 0; I < N; I++) {
            std::stringstream resSs;
            std::string pattern = patterns_[I];
            size_t i = 0;
            while (i < pattern.length()) {
                if (pattern[i] == '%' && i+1 < pattern.length()) {
                    switch (pattern[i+1]) {
                    case 'T' :
                        {
                            std::stringstream tagSs;
                            BOOST_FOREACH(std::string tag, edgeData.tags) {
                                if (!tagSs.str().empty()) {
                                    tagSs << tagSeparator_;
                                }
                                tagSs << tag;
                            }
                            resSs << tagSs.str();
                            i += 2;
                        }
                        break;
                    case 'c' :
                        {
                            resSs << edgeData.category;
                            i += 2;
                        }
                        break;
                    case 't' :
                        {
                            resSs << edgeData.text;
                            i += 2;
                        }
                        break;
                    case 'a' :
                        {
                            if (i+2 < pattern.length()) {
                                size_t from = i+3;
                                size_t to = pattern.find(close_(pattern[i+2]), from);
                                if (to != std::string::npos) {
                                    std::string attr = pattern.substr(from, to-from);
                                    resSs << edgeData.avMap[attr];
                                    i = to + 1;
                                } else {
                                    resSs << "%a";
                                    i += 2;
                                }
                            } else {
                                resSs << "%a";
                                i += 2;
                            }
                        }
                        break;
                    case 'A' :
                        {
                            std::stringstream avSs;
                            typedef std::pair<std::string, std::string> StrStrPair;
                            BOOST_FOREACH(StrStrPair avPair, edgeData.avMap) {
                                if (!avSs.str().empty()) {
                                    avSs << avPairsSeparator_;
                                }
                                avSs << avPair.first << avSeparator_ << avPair.second;
                            }
                            resSs << avSs.str();
                            i += 2;
                        }
                        break;
                    case 's' :
                        {
                            resSs << edgeData.score;
                            i += 2;
                        }
                        break;
                    case '*' :
                        {
                            if (i+2 < pattern.length()) {
                                size_t from = i+3;
                                size_t to = pattern.find(close_(pattern[i+2]), from);
                                if (pattern[to+1] == pattern[i+2]) {
                                    size_t from2 = to+2;
                                    size_t to2 = pattern.find(close_(pattern[i+2]), from2);
                                    if (to2 != std::string::npos) {
                                        std::string separator = pattern.substr(from, to-from);
                                        std::string subpattern = pattern.substr(from2, to2-from2);
                                        resSs << "TODO"; // TODO
                                        i = to2 + 1;
                                    } else {
                                        resSs << "%*";
                                        i += 2;
                                    }
                                } else {
                                    resSs << "%*";
                                    i += 2;
                                }
                            } else {
                                resSs << "%*";
                                i += 2;
                            }
                        }
                        break;
                    default :
                        {
                            resSs << '%';
                            i++;
                        }
                        break;
                    }
                } else {
                    resSs << pattern[i];
                    i++;
                }
            }
            resultElement[I] = resSs.str();
        }
        result.insert(resultElement);
    }
    return result;
}


#endif

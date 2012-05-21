#include "active_elements_printer.hpp"


#include <sstream>

#include <boost/foreach.hpp>


ActiveElementsPrinter ActiveElementsPrinter::Manager::getPrinter(std::string pattern) {
    return ActiveElementsPrinter(
        pattern,
        tagSeparator_,
        avPairsSeparator_,
        avSeparator_
    );
}


std::string ActiveElementsPrinter::print(
    EdgeData edgeData,
    std::string starText
) {
    return print(
        edgeData.tags,
        edgeData.category,
        edgeData.text,
        edgeData.avMap,
        edgeData.score,
        starText
    );
}


std::string ActiveElementsPrinter::print(
    std::set<std::string> tags,
    std::string category,
    std::string text,
    std::map<std::string, std::string> avMap,
    double score,
    std::string starText
) {
    std::stringstream result;
    size_t i = 0;
    while (i < pattern_.length()) {
        if (pattern_[i] == '%' && i+1 < pattern_.length()) {
            switch (pattern_[i+1]) {
            case 'T' :
                {
                    std::stringstream tagSs;
                    BOOST_FOREACH(std::string tag, tags) {
                        if (!tagSs.str().empty()) {
                            tagSs << tagSeparator_;
                        }
                        tagSs << tag;
                    }
                    result << tagSs.str();
                    i += 2;
                }
                break;
            case 'c' :
                {
                    result << category;
                    i += 2;
                }
                break;
            case 't' :
                {
                    result << text;
                    i += 2;
                }
                break;
            case 'a' :
                {
                    if (i+2 < pattern_.length()) {
                        size_t from = i+3;
                        size_t to = pattern_.find(close_(pattern_[i+2]), from);
                        if (to != std::string::npos) {
                            std::string attr = pattern_.substr(from, to-from);
                            result << avMap[attr];
                            i = to + 1;
                        } else {
                            result << "%a";
                            i += 2;
                        }
                    } else {
                        result << "%a";
                        i += 2;
                    }
                }
                break;
            case 'A' :
                {
                    std::stringstream avSs;
                    typedef std::pair<std::string, std::string> StrStrPair;
                    BOOST_FOREACH(StrStrPair avPair, avMap) {
                        if (!avSs.str().empty()) {
                            avSs << avPairsSeparator_;
                        }
                        avSs << avPair.first << avSeparator_ << avPair.second;
                    }
                    result << avSs.str();
                    i += 2;
                }
                break;
            case 's' :
                {
                    result << score;
                    i += 2;
                }
                break;
            case '*' :
                {
                    if (i+2 < pattern_.length()) {
                        size_t from = i+3;
                        size_t to = pattern_.find(close_(pattern_[i+2]), from);
                        if (pattern_[to+1] == pattern_[i+2]) {
                            size_t from2 = to+2;
                            size_t to2 = pattern_.find(close_(pattern_[i+2]), from2);
                            if (to2 != std::string::npos) {
                                std::string separator = pattern_.substr(from, to-from);
                                std::string subpattern = pattern_.substr(from2, to2-from2);
                                result << starText;
                                i = to2 + 1;
                            } else {
                                result << "%*";
                                i += 2;
                            }
                        } else {
                            result << "%*";
                            i += 2;
                        }
                    } else {
                        result << "%*";
                        i += 2;
                    }
                }
                break;
            default :
                {
                    result << '%';
                    i++;
                }
                break;
            }
        } else {
            result << pattern_[i];
            i++;
        }
    }
    return result.str();
}


char ActiveElementsPrinter::close_(char c) {
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

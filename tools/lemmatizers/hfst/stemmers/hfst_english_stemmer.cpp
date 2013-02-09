#include "hfst_english_stemmer.hpp"
#include "regexp.hpp"
#include <boost/algorithm/string.hpp>
#include "hfst_english_tags_parser.hpp"
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/string.hpp>

HfstEnglishStemmer::HfstEnglishStemmer(const boost::program_options::variables_map& options) {
    includeDerivatives_ = (options.count("include-derivatives") > 0) ? true : false;
}

HfstStems HfstEnglishStemmer::stem(const std::string &word, HfstAdapterInterface &hfstAdapter) {
    RegExp regAttribOrSuffix("\\+(.*)");
    RegExp regPrefix("(.*)\\+");
    RegExp regPos("\\[(.*)\\]");
    std::set<std::string> allowedAttributes;
    boost::copy( HfstEnglishTagsParser::PREDEFINED_TAGS 
            | boost::adaptors::map_keys, 
            std::inserter(allowedAttributes, allowedAttributes.begin()) );
    HfstStems result;
    HfstPaths hfstResults = hfstAdapter.analyzeWord(word);
    for (HfstPaths::iterator pathIt = hfstResults.begin(); 
            pathIt != hfstResults.end(); ++ pathIt) {
        std::vector<std::string> symbols = pathIt->path;
        std::string lemmaString = "";
        HfstRawTags tags;
        bool hasPrefix = false;
        bool hasDerivativeInfos = false;
        for (std::vector<std::string>::iterator symbolIt = symbols.begin(); 
                symbolIt != symbols.end(); ++ symbolIt) {
            std::string symbol = *symbolIt;
            boost::trim(symbol);
            if (symbol == "")
                continue;
            std::string tagValue;
            if (RegExp::FullMatch(symbol, regPrefix, &tagValue)) {
                hasDerivativeInfos = true;
                if (includeDerivatives_) {
                    lemmaString = tagValue;
                    hasPrefix = true;
                    continue;
                } else
                    break;
            }
            if (RegExp::FullMatch(symbol, regAttribOrSuffix, &tagValue)) {
                std::string tagToCheck = tagValue;
                boost::algorithm::to_lower(tagToCheck);
                std::set<std::string>::iterator foundAttribute = allowedAttributes.find(tagToCheck);
                if (foundAttribute != allowedAttributes.end()) {
                    tags.push_back(tagValue);
                } else {
                    hasDerivativeInfos = true;
                    if (includeDerivatives_) {
                        result.insert(std::pair<std::string, std::vector<std::string> >(lemmaString, tags));
                        lemmaString = tagValue;
                        tags.clear();
                    } else
                        break;
                }
                continue;
            }
            if (RegExp::FullMatch(symbol, regPos, &tagValue)) {
                tags.push_back(tagValue);
                continue;
            }
            if (hasPrefix) {
                result.insert(std::pair<std::string, std::vector<std::string> >(lemmaString, tags));
                lemmaString = "";
                tags.clear();
                hasPrefix = false;
            }
            lemmaString += symbol;
        }
        if (lemmaString != "") {
            if (includeDerivatives_ || !hasDerivativeInfos) {
                std::string lemma = lemmaString;
                result.insert(std::pair<std::string, std::vector<std::string> >(lemma, tags));
            }
        }

    }
    return result;
}

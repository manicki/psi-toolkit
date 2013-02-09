#include "hfst_finnish_stemmer.hpp"
#include "regexp.hpp"
#include <boost/algorithm/string.hpp>

const std::string HfstFinnishStemmer::HFST_COMPOUND_BOUNDARY = "COMPOUND";

HfstFinnishStemmer::HfstFinnishStemmer(const boost::program_options::variables_map& options) {
    includeCompounds_ = (options.count("include-compounds") > 0) ? true : false;
}

HfstStems HfstFinnishStemmer::stem(const std::string &word, HfstAdapterInterface &hfstAdapter) {
    RegExp regFlag("@.+@");
    RegExp regAttrib("\\[.+\\]");
    RegExp regExtraAttrib("([^\\[\\]]+)(\\[.+\\])");
    RegExp regBoundary("\\[BOUNDARY=([^=]+)\\]");
    HfstStems result;
    HfstPaths hfstResults = hfstAdapter.analyzeWord(word);
    for (HfstPaths::iterator pathIt = hfstResults.begin(); 
            pathIt != hfstResults.end(); ++ pathIt) {
        std::vector<std::string> symbols = pathIt->path;
        std::string lemmaString = "";
        HfstRawTags tags;
        for (std::vector<std::string>::iterator symbolIt = symbols.begin(); 
                symbolIt != symbols.end(); ++ symbolIt) {
            std::string symbol = *symbolIt;
            boost::trim(symbol);
            if (symbol == "")
                continue;
            std::string boundaryType = "";
            if (RegExp::FullMatch(symbol, regBoundary, &boundaryType)) {
                if (!excludeInterpretation_(boundaryType)) {
                    if (lemmaString != "") {
                        std::string lemma = lemmaString;
                        std::string extraTag;
                        if (RegExp::FullMatch(lemmaString, regExtraAttrib, &lemma, &extraTag))
                            tags.push_back(extraTag);
                        result.insert(std::pair<std::string, std::vector<std::string> >(lemma, tags));
                    }
                } else
                    break;
                lemmaString = "";
                tags.clear();
                continue;
            }
            if (RegExp::FullMatch(symbol, regFlag) || RegExp::FullMatch(symbol, regAttrib)) {
                tags.push_back(symbol);
                continue;
            }
            lemmaString += symbol;
        }
    }
    return result;
}

bool HfstFinnishStemmer::excludeInterpretation_(const std::string &boundaryType) {
    if (boundaryType == HFST_COMPOUND_BOUNDARY)
        if (!includeCompounds_)
            return true;
    return false;
}

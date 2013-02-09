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
    RegExp regBoundary("\\[BOUNDARY=([^=]+)\\]");
    std::string strLemmaStart("[LEMMA='");
    std::string strLemmaEnd("']");
    std::string strCompoundPartBoundary = "'|'";
  
    HfstStems result;
    HfstPaths hfstResults = hfstAdapter.analyzeWord(word);
    for (HfstPaths::iterator pathIt = hfstResults.begin(); 
            pathIt != hfstResults.end(); ++ pathIt) {
        std::vector<std::string> symbols = pathIt->path;
        std::string lemmaString = "";
        std::string extraTagString = "";
        bool inLemma = false;
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
                        if (RegExp::FullMatch(extraTagString, regAttrib))
                            tags.push_back(extraTagString);
                        result.insert(std::pair<std::string, std::vector<std::string> >(lemmaString, tags));
                    }
                } else
                    break;
                lemmaString = "";
                extraTagString = "";
                tags.clear();
                continue;
            }
            if (symbol == strLemmaStart) {
                inLemma = true;
                continue;
            }
            if (symbol == strLemmaEnd) {
                inLemma = false;
                continue;
            }
            if (RegExp::FullMatch(symbol, regFlag) || RegExp::FullMatch(symbol, regAttrib)) {
                tags.push_back(symbol);
                continue;
            }
            if (symbol == strCompoundPartBoundary)
                continue;
            if (inLemma)
                lemmaString += symbol;
            else {
                if (symbol != "%") //@todo: why there is [KTN=6%0]? 
                    extraTagString += symbol;
            }
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

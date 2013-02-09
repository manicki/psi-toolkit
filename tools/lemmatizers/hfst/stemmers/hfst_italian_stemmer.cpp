#include "hfst_italian_stemmer.hpp"
#include "regexp.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/string.hpp>

HfstItalianStemmer::HfstItalianStemmer(const boost::program_options::variables_map& options) {
}

HfstStems HfstItalianStemmer::stem(const std::string &word, HfstAdapterInterface &hfstAdapter) {
    RegExp regTag("<(.*)>");
    std::string lemmaBoundary = "#";
    std::string lexemeBoundary = ":";
    std::string lexemePosBoundary = "-";
    std::string tagBoundary = "+";
    HfstStems result;
    HfstPaths hfstResults = hfstAdapter.analyzeWord(word);
    for (HfstPaths::iterator pathIt = hfstResults.begin(); 
            pathIt != hfstResults.end(); ++ pathIt) {
        std::vector<std::string> symbols = pathIt->path;
        std::string lemmaString = "";
        std::string tagString = "";
        HfstRawTags tags;
        bool lemmaSet = false;
        for (std::vector<std::string>::iterator symbolIt = symbols.begin(); 
                symbolIt != symbols.end(); ++ symbolIt) {
            std::string symbol = *symbolIt;
            boost::trim(symbol);
            if (symbol == "")
                continue;
            if (symbol == lemmaBoundary) {
                lemmaSet = true;
                continue;
            }
            if (symbol == lexemeBoundary || symbol == lexemePosBoundary
                    || symbol == tagBoundary) {
                if (tagString != "") {
                    tags.push_back(tagString);
                    tagString = "";
                    if (symbol == lexemePosBoundary)
                        tagString = lexemePosBoundary;
                }
                continue;
            }
            if (!lemmaSet)
                lemmaString += symbol;
            else
                tagString += symbol;
        }
        if (tagString != "")
            tags.push_back(tagString);
        if (lemmaString != "") {
            result.insert(std::pair<std::string, std::vector<std::string> >(lemmaString, tags));
        }

    }
    return result;
}

#include "hfst_german_stemmer.hpp"
#include "regexp.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/string.hpp>

HfstGermanStemmer::HfstGermanStemmer(const boost::program_options::variables_map& options) {
}

HfstStems HfstGermanStemmer::stem(const std::string &word, HfstAdapterInterface &hfstAdapter) {
    RegExp regTag("<(.*)>");
    HfstStems result;
    HfstPaths hfstResults = hfstAdapter.analyzeWord(word);
    for (HfstPaths::iterator pathIt = hfstResults.begin(); 
            pathIt != hfstResults.end(); ++ pathIt) {
        std::vector<std::string> symbols = pathIt->path;
        std::string lemmaString = "";
        HfstRawTags tags;
        bool lemmaSet = false;
        bool tagsSet = false;
        for (std::vector<std::string>::iterator symbolIt = symbols.begin(); 
                symbolIt != symbols.end(); ++ symbolIt) {
            std::string symbol = *symbolIt;
            boost::trim(symbol);
            if (symbol == "")
                continue;
            std::string tagValue;
            if (RegExp::FullMatch(symbol, regTag, &tagValue)) {
                tags.push_back(symbol);
                tagsSet = true;
                continue;
            }
            if (lemmaSet && tagsSet) {
                result.insert(std::pair<std::string, std::vector<std::string> >(lemmaString, tags));
                lemmaString = "";
                tags.clear();
                lemmaSet = false;
                tagsSet = false;
            }
            lemmaString += symbol;
            lemmaSet = true;
        }
        if (lemmaString != "") {
            result.insert(std::pair<std::string, std::vector<std::string> >(lemmaString, tags));
        }

    }
    return result;
}

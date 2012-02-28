#include "sfst_lemmatizer.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>

std::string sfstLemmatizer::tagSeparator = "+";

sfstLemmatizer::sfstLemmatizer(const boost::program_options::variables_map& options)
    : annotationManager(NULL), level(3)
{
    if (options.count("level") > 0) {
        setLevel(options["level"].as<int>());
    }

    //initializePolishStemmer();
    //initializeList();
    //initializeWordData();
    //initializeString();
}

std::string sfstLemmatizer::getName() {
    return "sfstLemmatizer";
}

std::string sfstLemmatizer::getLanguage() const {
    return "tr";
}

void sfstLemmatizer::setLevel(int lvl) {
    if (0 <= lvl && lvl <= 0) {
        level = lvl;
    }
}

std::vector<std::string> sfstLemmatizer::simpleStem(const std::string & word) {

    std::vector<std::string> result;
    return result;
}

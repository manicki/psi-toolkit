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

    initializeTurkishTransducer();
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

void sfstLemmatizer::initializeTurkishTransducer() {
    FILE *file;
    
    //turkishTransducer = NULL;

    if ((file = fopen("data/tr/trmorph-0.2.1.a","rb")) == NULL) {
        Transducer tmpTrans(file);
        fclose(file);
		turkishTransducer = tmpTrans;
    }
    else {
        ERROR("The trmormp's file has been not found");
    }
	
}

std::vector<std::string> sfstLemmatizer::simpleStem(const std::string & word) {

    std::vector<std::string> result;
    return result;
}

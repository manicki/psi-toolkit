#include "sfst_lemmatizer.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>

std::string SfstLemmatizer::tagSeparator = "+";

SfstLemmatizer::SfstLemmatizer(const boost::program_options::variables_map& options)
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

std::string SfstLemmatizer::getName() {
    return "sfstLemmatizer";
}

std::string SfstLemmatizer::getLanguage() const {
    return "tr";
}

void SfstLemmatizer::setLevel(int lvl) {
    if (0 <= lvl && lvl <= 0) {
        level = lvl;
    }
}

void SfstLemmatizer::initializeTurkishTransducer() {
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

std::vector<std::string> SfstLemmatizer::simpleStem(const std::string & word) {

    std::vector<std::string> result;

	std::streambuf* oldBuf = std::cout.rdbuf();
	std::ostringstream stdoutCap;
	std::cout.rdbuf( stdoutCap.rdbuf() );
    
	if (turkishTransducer.analyze_string((char*)word.c_str(), stdout, true)) {
		
		size_t pos;
		
		std::string output = stdoutCap.str();

		while((pos = output.find_first_of('\n')) != -1) {
			std::string stem = output.substr(0, pos);
			output = output.substr(pos + 1);
			if ((pos = stem.find_first_of('<')) == -1)
				result.insert(result.begin(), stem);
			else 
				result.insert(result.begin(), stem.substr(0, pos));
			}
		
	}
    
	std::cout.rdbuf( oldBuf );
    
    return result;
}

#include "model_language.hpp"

#include "processor_file_fetcher.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

ModelLanguage::ModelLanguage(std::string lang, std::string file, std::string nonStandardLetters)
    : name(lang), letters(nonStandardLetters) {

    initBigramFrequencyTable(file);
}

void ModelLanguage::initBigramFrequencyTable(std::string file) {

    ProcessorFileFetcher fileFetcher(__FILE__);
    boost::filesystem::path pathToLangFile = fileFetcher.getOneFile("%ITSDATA%/" + file);

    std::ifstream langFile(pathToLangFile.string().c_str(), std::ifstream::in);

    int counter = 0;
    char comma; // FIXME: the last sign in each line is an unnecessary comma

    while (langFile.good() && counter < FTABLE_LENGTH) {
        langFile >> bigramFrequencyTable[counter++] >> comma;
    }

    langFile.close();
}

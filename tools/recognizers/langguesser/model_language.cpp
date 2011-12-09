#include "model_language.hpp"

#include "processor_file_fetcher.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

ModelLanguage::ModelLanguage(std::string lang, std::string file, std::string nonStandardLetters)
    : name(lang), letters(nonStandardLetters) {

    initFrequencyTable(file);
}

void ModelLanguage::initFrequencyTable(std::string file) {

    ProcessorFileFetcher fileFetcher(__FILE__);
    boost::filesystem::path pathToLangFile = fileFetcher.getOneFile("%ITSDATA%/" + file);

    std::ifstream langFile(pathToLangFile.string().c_str(), std::ifstream::in);

    int counter = 0;
    char comma; // FIXME: the last sign in each line is an unnecessary comma

    while (langFile.good() && counter < FTABLE_LENGTH) {
        langFile >> frequencyTable[counter++] >> comma;
    }

    langFile.close();
}

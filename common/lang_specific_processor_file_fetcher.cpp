#include "lang_specific_processor_file_fetcher.hpp"

LangSpecificProcessorFileFetcher::LangSpecificProcessorFileFetcher(
    boost::filesystem::path sourceFilePath, const std::string& lang)
    :ProcessorFileFetcher(sourceFilePath) {
    addParam(LANG_PARAM, lang);
}

const std::string LangSpecificProcessorFileFetcher::LANG_PARAM = "LANG";

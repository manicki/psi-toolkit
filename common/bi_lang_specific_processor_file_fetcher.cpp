#include "bi_lang_specific_processor_file_fetcher.hpp"

BiLangSpecificProcessorFileFetcher::BiLangSpecificProcessorFileFetcher(
    boost::filesystem::path sourceFilePath,
    const std::string& lang,
    const std::string& trg_lang)
    :LangSpecificProcessorFileFetcher(sourceFilePath, lang) {
    addParam(TRG_LANG_PARAM, trg_lang);
}

const std::string BiLangSpecificProcessorFileFetcher::TRG_LANG_PARAM = "TRGLANG";

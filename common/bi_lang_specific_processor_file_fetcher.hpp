#ifndef BI_LANG_SPECIFIC_PROCESSOR_FILE_FETCHER_HDR
#define BI_LANG_SPECIFIC_PROCESSOR_FILE_FETCHER_HDR

#include "lang_specific_processor_file_fetcher.hpp"

class BiLangSpecificProcessorFileFetcher : public LangSpecificProcessorFileFetcher {
public:
    BiLangSpecificProcessorFileFetcher(boost::filesystem::path sourceFilePath,
                                       const std::string& lang,
                                       const std::string& trg_lang);

private:
    static const std::string TRG_LANG_PARAM;
};

#endif

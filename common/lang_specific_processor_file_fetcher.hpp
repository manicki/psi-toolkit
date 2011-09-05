#ifndef LANG_SPECIFIC_PROCESSOR_FILE_FETCHER_HDR
#define LANG_SPECIFIC_PROCESSOR_FILE_FETCHER_HDR

#include "processor_file_fetcher.hpp"

class LangSpecificProcessorFileFetcher : public ProcessorFileFetcher {

public:
    LangSpecificProcessorFileFetcher(boost::filesystem::path sourceFilePath,
                                     const std::string& lang);

private:
    static const std::string LANG_PARAM;

};


#endif

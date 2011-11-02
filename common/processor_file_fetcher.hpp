#ifndef PROCESSOR_FILE_FETCHER_HDR
#define PROCESSOR_FILE_FETCHER_HDR

#include <boost/foreach.hpp>

#include "file_fetcher.hpp"

class ProcessorFileFetcher : public FileFetcher {

public:
    explicit ProcessorFileFetcher(boost::filesystem::path sourceFilePath);

private:
    void initDirectoryParams_(boost::filesystem::path sourceFilePath);

    boost::filesystem::path underscores2minuses_(
        const boost::filesystem::path& segment);
};

#endif

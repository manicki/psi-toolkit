#include "processor_file_fetcher.hpp"

#include "its_data.hpp"

ProcessorFileFetcher::ProcessorFileFetcher(boost::filesystem::path sourceFilePath) {
    initDirectoryParams_(sourceFilePath);
}

void ProcessorFileFetcher::initDirectoryParams_(boost::filesystem::path sourceFilePath) {
    addParam("ITSDATA",  getItsData(sourceFilePath).string());
}

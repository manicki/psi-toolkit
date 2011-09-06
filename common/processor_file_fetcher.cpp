#include "processor_file_fetcher.hpp"

ProcessorFileFetcher::ProcessorFileFetcher(boost::filesystem::path sourceFilePath) {
    initDirectoryParams_(sourceFilePath);
}

void ProcessorFileFetcher::initDirectoryParams_(boost::filesystem::path sourceFilePath) {

    if (boost::filesystem::is_regular_file(sourceFilePath))
        sourceFilePath = sourceFilePath.parent_path();

    sourceFilePath = sourceFilePath.relative_path();

    boost::filesystem::path sourcePath;
    bool found = false;

    for (boost::filesystem::path::iterator it = sourceFilePath.begin();
         it != sourceFilePath.end();
         ++it) {
        // trick for compilation with both Boost 1.42 and 1.47
        // (in Boost 1.42 strings are returned by an iterator rather
        //  than boost::filesystem::paths)
        boost::filesystem::path seg(*it);

        if (seg.string() == "tools"
            || seg.string() == "formatters"
            || seg.string() == "server") {
            sourcePath /= (*it);
            found = true;
        }
        else if (found)
            sourcePath /= (*it);
    }

    if (!found)
        throw Exception(std::string("unexpected source path `") + sourceFilePath.string() + "'");

    boost::filesystem::path itsData = getRootDir_() / sourcePath / "data";

    addParam("ITSDATA",  itsData.string());
}

#include "processor_file_fetcher.hpp"

#include "configurator.hpp"

ProcessorFileFetcher::ProcessorFileFetcher(boost::filesystem::path sourceFilePath) {
    initDirectoryParams_(sourceFilePath);
}

void ProcessorFileFetcher::initDirectoryParams_(boost::filesystem::path sourceFilePath) {

    boost::filesystem::path lastComponent;

    bool takenFromSourceFile = false;

    if (boost::filesystem::is_regular_file(sourceFilePath)) {
        lastComponent = sourceFilePath.filename();
        takenFromSourceFile = true;

        sourceFilePath = sourceFilePath.parent_path();
    }

    sourceFilePath = sourceFilePath.relative_path();

    boost::filesystem::path sourcePath;
    bool found = false;

    BOOST_FOREACH(boost::filesystem::path i, sourceFilePath) {
        // trick for compilation with both Boost 1.42 and 1.47
        // (in Boost 1.42 strings are returned by an iterator rather
        //  than boost::filesystem::paths)
        boost::filesystem::path seg(i);

        if (seg.string() == "tools"
            || seg.string() == "formatters"
            || seg.string() == "server") {
            sourcePath /= (i);
            found = true;
        }
        else if (found)
            sourcePath /= (i);

        if (!takenFromSourceFile)
            lastComponent = seg;
    }

    if (!found)
        throw Exception(std::string("unexpected source path `") + sourceFilePath.string() + "'");

    boost::filesystem::path itsData =
        (Configurator::getInstance().isRunAsInstalled()
         ? getDataDir_() / underscores2minuses_(lastComponent)
         : boost::filesystem::path("..") / sourcePath / "data");

    addParam("ITSDATA",  itsData.string());
}

boost::filesystem::path ProcessorFileFetcher::underscores2minuses_(
    const boost::filesystem::path& segment) {
    std::string segmentAsString = segment.string();
    std::replace(segmentAsString.begin(), segmentAsString.end(), '_', '-');

    return boost::filesystem::path(segmentAsString);
}

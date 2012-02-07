#ifndef TEST_EXTRACTOR_HDR
#define TEST_EXTRACTOR_HDR

#include <vector>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

#include "test_batch.hpp"

class TestExtractor {

public:

    TestExtractor();

    void lookForTestBatches(std::vector<boost::filesystem::path> directories,
                            const std::string& prefix);
    void clearTestBatches();
    std::vector<TestBatch> & getTestBatches();

private:

    std::string directoryPrefix_;

    void lookForTestBatches_(const boost::filesystem::path& directory);
    void lookInDirectory_(const boost::filesystem::path& directory);
    void addTestBatch_(const boost::filesystem::path& directory);

    void parseFilePath_(const boost::filesystem::path& filePath,
                        std::string& basename,
                        std::string& infix,
                        std::string& suffix);

    void checkFileMap_(std::map<std::string, boost::filesystem::path>& filemap,
                       const boost::filesystem::path& path,
                       const std::string& key,
                       const std::string& infoString);

    std::string readCommand_(boost::filesystem::path commandFileName);
    std::string stripComments_(const std::string& line);

    std::vector<TestBatch> testBatches_;

    const static boost::filesystem::path TEST_BATCH_DIRECTORY_NAME;
    const static boost::filesystem::path TEST_COMMAND_FILE_NAME;
    const static boost::filesystem::path TEST_DESCRIPTION_FILE_NAME;
};

#endif

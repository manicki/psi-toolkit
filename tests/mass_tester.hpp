#ifndef MASS_TESTER_HDR
#define MASS_TESTER_HDR

#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/filesystem.hpp>

#include "test_batch.hpp"

class MassTester {

public:
    MassTester(int argc, char* argv[]);

    int run();

private:

    static boost::program_options::options_description optionsHandled();

    void lookForTestBatches_(const boost::filesystem::path& directory);
    void lookInDirectory_(const boost::filesystem::path& directory);
    void addTestBatch_(const boost::filesystem::path& directory);
    void runAllBatches_();
    void runBatch_(const TestBatch& batch);

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

    std::vector<boost::filesystem::path> directories_;

    std::vector<TestBatch> testBatches_;

    const static boost::filesystem::path TEST_BATCH_DIRECTORY_NAME;
    const static boost::filesystem::path TEST_COMMAND_FILE_NAME;
};

#endif

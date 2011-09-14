#ifndef MASS_TESTER_HDR
#define MASS_TESTER_HDR

#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/filesystem.hpp>

class MassTester {

public:
    MassTester(int argc, char* argv[]);

    int run();

private:
    struct TestRun {
        boost::filesystem::path inputFilePath;
        boost::filesystem::path expectedOutputFilePath;
    };

    class TestBatch {
    private:
        boost::filesystem::path mDirectory_;
        std::string pipeline_;
        std::vector<TestRun> testRuns_;
    public:
        TestBatch(const boost::filesystem::path& mDirectory,
                  const std::string& pipeline)
            :mDirectory_(mDirectory), pipeline_(pipeline) {
        }

        boost::filesystem::path getDirectory() const {
            return mDirectory_;
        }

        std::string getPipeline() const {
            return pipeline_;
        }

        void addTestRun(const TestRun& testRun) {
            testRuns_.push_back(testRun);
        }


    };

    static boost::program_options::options_description optionsHandled();

    void lookForTestBatches_(const boost::filesystem::path& directory);
    void lookInDirectory_(const boost::filesystem::path& directory);
    void addTestBatch_(const boost::filesystem::path& directory);
    void runAllBatches_();
    void runBatch_(const TestBatch& batch);

    std::string readCommand_(boost::filesystem::path commandFileName);
    std::string stripComments_(const std::string& line);

    std::vector<boost::filesystem::path> directories_;

    std::vector<TestBatch> testBatches_;

    const static boost::filesystem::path TEST_BATCH_DIRECTORY_NAME;
    const static boost::filesystem::path TEST_COMMAND_FILE_NAME;
};

#endif

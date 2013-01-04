#ifndef TEST_BATCH_HDR
#define TEST_BATCH_HDR

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <string>
#include <vector>

class TestRun {
private:
    boost::filesystem::path inputFilePath_;
    boost::filesystem::path expectedOutputFilePath_;
public:
    TestRun(const boost::filesystem::path& inputFilePath,
            const boost::filesystem::path& expectedOutputFilePath)
        : inputFilePath_(inputFilePath), expectedOutputFilePath_(expectedOutputFilePath) {
    }

    boost::filesystem::path getInputFilePath() const {
        return inputFilePath_;
    }

    boost::filesystem::path getExpectedOutputFilePath() const {
        return expectedOutputFilePath_;
    }
};

class TestBatch {
private:
    boost::filesystem::path mDirectory_;
    std::string pipeline_;
    std::string description_;
    std::vector<TestRun> testRuns_;
    bool exampleOnly_;
public:
    TestBatch(const boost::filesystem::path& mDirectory,
              const std::string& pipeline,
              const std::string& description,
              bool exampleOnly = false)
        : mDirectory_(mDirectory),
        description_(description),
        exampleOnly_(exampleOnly) {
        pipeline_ = processPipeline_(pipeline);
    }

    boost::filesystem::path getDirectory() const {
        return mDirectory_;
    }

    std::string getPipeline() const {
        return pipeline_;
    }

    std::string getDescription() const {
        return description_;
    }

    void addTestRun(const TestRun& testRun) {
        testRuns_.push_back(testRun);
    }

    template<class Out>
    void getTestRuns(Out outputIterator) const {
        BOOST_FOREACH(const TestRun& testRun, testRuns_) {
            *outputIterator++ = testRun;
        }
    }

    std::vector<TestRun> getTestRuns() const {
        return testRuns_;
    }

    bool isExampleOnly() const {
        return exampleOnly_;
    }

private:
    std::string processPipeline_(const std::string& pipeline);
};

#endif

#include "batch_runner.hpp"

#include <cassert>
#include <boost/filesystem/fstream.hpp>
#include <sstream>

#include "logging.hpp"

BatchRunner::BatchRunner(const TestBatch& testBatch)
    :testBatch_(testBatch), pipeRunner_(testBatch.getPipeline()) {

    testBatch.getTestRuns(std::back_inserter(testRuns_));

    testStates_.reset(new std::vector<test_state>(testRuns_.size(), INTACT));

    INFO(testRuns_.size() << " tests");


}

bool BatchRunner::runSingleTest() {
    size_t testIx = getNextTest_();

    if (testIx < testRuns_.size()) {
        runTest_(testIx);
        return true;
    }

    return false;
}

size_t BatchRunner::getNextTest_() {
    assert (testRuns_.size() == testStates_->size());

    for (size_t i = 0; i < testStates_->size(); ++i)
        if ((*testStates_)[i] == INTACT) {
            (*testStates_)[i] = RUNNING;
            return i;
        }

    return testStates_->size();
}

void BatchRunner::runTest_(size_t testIx) {
    const TestRun& testRun(testRuns_[testIx]);

    INFO("running test for " << testRun.getInputFilePath());

    boost::filesystem::ifstream inputStream(testRun.getInputFilePath());

    std::ostringstream oss;
    pipeRunner_.run(inputStream, oss);

    std::string got = oss.str();

    std::string expected = slurpFile_(testRun.getExpectedOutputFilePath());

    if (got == expected) {
        INFO("...OK");
    }
    else {
        INFO("... FAILED (unexpected output)");
    }
}

std::string BatchRunner::slurpFile_(const boost::filesystem::path& filePath) {
    boost::filesystem::ifstream ifs(filePath, std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(&bytes[0], fileSize);

    return std::string(&bytes[0], fileSize);
}



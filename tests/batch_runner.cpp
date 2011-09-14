#include "batch_runner.hpp"

#include <cassert>
#include "logging.hpp"

BatchRunner::BatchRunner(const TestBatch& testBatch)
    :testBatch_(testBatch) {

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
}


#ifndef BATCH_RUNNER_HDR
#define BATCH_RUNNER_HDR

#include <boost/scoped_ptr.hpp>

#include "test_batch.hpp"

class BatchRunner {

public:
    BatchRunner(const TestBatch& testBatch);

    bool runSingleTest();

private:
    size_t getNextTest_();
    void runTest_(size_t testIx);

    const TestBatch& testBatch_;
    enum test_state {
        INTACT,
        RUNNING,
        FAILED,
        SUCCEEDED };

    std::vector<TestRun> testRuns_;
    boost::scoped_ptr<std::vector<test_state> > testStates_;
};

#endif

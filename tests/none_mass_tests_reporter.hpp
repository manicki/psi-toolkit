#ifndef NONE_MASS_TESTS_REPORTER
#define NONE_MASS_TESTS_REPORTER

#include <iostream>

#include "mass_tests_reporter.hpp"

class NoneMassTestsReporter : public MassTestsReporter {

private:
    std::ostream& outStream_;
    size_t tests_;
    size_t failures_;

public:
    NoneMassTestsReporter(std::ostream& outStream)
        :outStream_(outStream), tests_(0U), failures_(0U) {
    }

    virtual void report(
        const std::string& /*batchName*/,
        const std::string& /*testRunName*/,
        bool success) {
        ++tests_;
        if (!success)
            ++failures_;
    }

    virtual void finish() {
        outStream_ << tests_ << " tests run, " << failures_ << " failures " << std::endl;
    }
};

#endif

#ifndef NONE_MASS_TESTS_REPORTER
#define NONE_MASS_TESTS_REPORTER

#include <iostream>

#include "mass_tests_reporter.hpp"

class NoneMassTestsReporter : public MassTestsReporter {

public:
    virtual void report(
        const std::string& /*batchName*/,
        const std::string& /*testRunName*/,
        bool /*success*/) {}

    virtual void finish() {}
};

#endif

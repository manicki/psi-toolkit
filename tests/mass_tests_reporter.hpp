#ifndef MASS_TESTS_REPORTER
#define MASS_TESTS_REPORTER

class MassTestsReporter {
public:
    virtual void report(
        const std::string& batchName,
        const std::string& testRunName,
        bool success) = 0;

    virtual void finish() = 0;

    virtual ~MassTestsReporter() {
    }
};

#endif

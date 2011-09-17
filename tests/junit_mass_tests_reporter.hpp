#ifndef JUNIT_MASS_TESTS_REPORTER
#define JUNIT_MASS_TESTS_REPORTER

#include <iostream>
#include <vector>

#include "mass_tests_reporter.hpp"

class JUnitMassTestsReporter : public MassTestsReporter {

public:
    JUnitMassTestsReporter(std::ostream& outStream);

    virtual void report(
        const std::string& batchName,
        const std::string& testRunName,
        bool success);

    virtual void finish();

private:
    struct TestInfo {
        std::string batchName_;
        std::string testRunName_;
        bool success_;

        TestInfo(const std::string& batchName,
                 const std::string& testRunName,
                 bool success)
            :batchName_(batchName), testRunName_(testRunName), success_(success) {
        }
    };

    void printXmlHeader_() const;
    void printTestSuite_(size_t tests, size_t failures);
    void printTestCases_();
    void printTestCase_(const TestInfo& testInfo);
    void printTestSuiteEnd_() const;


    std::string quoteXmlEntities_(const std::string& t);

    std::ostream& outStream_;


    std::vector<TestInfo> testInfos_;
    size_t failures;

    const static std::string INDENTATION_;
};

#endif

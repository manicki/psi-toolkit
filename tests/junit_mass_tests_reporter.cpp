#include "junit_mass_tests_reporter.hpp"

#include <boost/foreach.hpp>

JUnitMassTestsReporter::JUnitMassTestsReporter(std::ostream& outStream)
    :outStream_(outStream), failures(0U) {
}

void JUnitMassTestsReporter::report(
        const std::string& batchName,
        const std::string& testRunName,
        bool success) {
    testInfos_.push_back(TestInfo(batchName, testRunName, success));
    if (!success)
        ++failures;
}

void JUnitMassTestsReporter::finish() {
    printXmlHeader_();
    printTestSuite_(testInfos_.size(), failures);
    printTestCases_();
    printTestSuiteEnd_();
}

void JUnitMassTestsReporter::printXmlHeader_() const {
    outStream_ << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
}

void JUnitMassTestsReporter::printTestSuite_(size_t tests, size_t failures) {
    outStream_ << "<testsuite errors=\"0\" failures=\""
               << failures
               << "\" hostname=\"localhost\" name=\"mass-tests\" tests=\""
               << tests
               << "\" time=\"1.00\" timestamp=\"2011-11-02T23:59:59\">" << std::endl;
}

void JUnitMassTestsReporter::printTestCases_() {
    BOOST_FOREACH(const TestInfo& testInfo, testInfos_) {
        printTestCase_(testInfo);
    }
}

void JUnitMassTestsReporter::printTestCase_(const TestInfo& testInfo) {
    outStream_ << INDENTATION_
               << "<testcase classname=\""
               << testInfo.batchName_
               << "\" name=\""
               << testInfo.testRunName_
               << "\" time=\"1.0000\"";

    if (testInfo.success_)
        outStream_ << " />" << std::endl;
    else {
        outStream_ << ">" << std::endl;
        outStream_ << INDENTATION_ << INDENTATION_
                   << "<failure>unexpected output</failure>" << std::endl;
        outStream_ << INDENTATION_ << "</testcase>" << std::endl;
    }
}

void JUnitMassTestsReporter::printTestSuiteEnd_() const {
    outStream_ << "</testsuite>" << std::endl;
}

std::string JUnitMassTestsReporter::quoteXmlEntities_(const std::string& t) {
    std::string ret;

    BOOST_FOREACH(const char ch, t) {
        switch (ch) {
        case '&': ret += "&amp;"; break;
        case '"': ret += "&quote;"; break;
        case '\'': ret += "&apos;"; break;
        case '<': ret += "&lt;"; break;
        case '>': ret += "&gt;"; break;
        default:
            ret += ch;
        }
    }

    return ret;
}

const std::string JUnitMassTestsReporter::INDENTATION_ = "    ";

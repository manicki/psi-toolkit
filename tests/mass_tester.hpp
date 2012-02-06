#ifndef MASS_TESTER_HDR
#define MASS_TESTER_HDR

#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

#include "test_extractor.hpp"
#include "mass_tests_reporter.hpp"
#include "none_mass_tests_reporter.hpp"
#include "junit_mass_tests_reporter.hpp"

class MassTester {

public:

    MassTester(int argc, char* argv[]);

    int run();

private:

    static boost::program_options::options_description optionsHandled();

    TestExtractor testExtractor_;
    void runAllBatches_();
    void runBatch_(const TestBatch& batch);

    std::vector<boost::filesystem::path> directories_;

    boost::scoped_ptr<std::ostream> outStream_;
    boost::scoped_ptr<MassTestsReporter> reporter_;
};

#endif

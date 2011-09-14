#ifndef MASS_TESTER_HDR
#define MASS_TESTER_HDR

#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/filesystem.hpp>

class MassTester {

public:
    MassTester(int argc, char* argv[]);

    int run();

private:
    static boost::program_options::options_description optionsHandled();

    std::vector<boost::filesystem::path> directories_;
};

#endif

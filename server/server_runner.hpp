#ifndef SERVER_RUNNER_HDR
#define SERVER_RUNNER_HDR

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <sstream>

class ServerRunner {

public:
    ServerRunner(int argc, char* argv[]);
    int run();

private:
    boost::program_options::variables_map options_;
    std::vector<std::string> annotatorOptions_;

    boost::program_options::options_description optionsDescription_;

    boost::program_options::variables_map parseOptions_(int argc, char * argv[]);
    void setOptionsDescription_();
    std::string annotatorOptionsAsString_();

    bool stopAfterExecutingOptions_();

    void daemonize_(bool leaveStandardDescriptors);
    int setRootDirectory_();

    boost::filesystem::path rootDir_;

    static const std::string DEFAULT_PIPE;
    static const std::string DEFAULT_TEXT;
};

#endif

#ifndef SERVER_RUNNER_HDR
#define SERVER_RUNNER_HDR

#include <boost/program_options.hpp>

#include "index_site.hpp"
#include "pipe_site.hpp"

class ServerRunner {

public:
    ServerRunner(int argc, char* argv[]);
    int run();

private:
	boost::program_options::variables_map options;
	std::vector<std::string> annotatorOptions;

	boost::program_options::options_description optionsDescription;

	boost::program_options::variables_map parseOptions(int argc, char * argv[]);
	void setOptionsDescription();
	std::string annotatorOptionsAsString();

	int executeOptions();

	static const std::string DEFAULT_PIPE;
};

#endif

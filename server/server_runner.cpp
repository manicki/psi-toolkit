#include "server_runner.hpp"

ServerRunner::ServerRunner(int argc, char * argv[]) 
	: optionsDescription_("PsiServer options") {

	options_ = parseOptions(argc, argv);
}

boost::program_options::variables_map ServerRunner::parseOptions(int argc, char * argv[]) {

	setOptionsDescription();

	boost::program_options::variables_map vmap;
	boost::program_options::parsed_options opts = 
		boost::program_options::command_line_parser(argc, argv).
		options(optionsDescription_).
		allow_unregistered().
		run();

	boost::program_options::store(opts, vmap);
	boost::program_options::notify(vmap);

	// rest options needed for the further annotators
	// std::vector<std::string> rest_opts = boost::program_options::collect_unrecognized(
	// 	opts.options, boost::program_options::include_positional);

	return vmap;
}

void ServerRunner::setOptionsDescription() {

	optionsDescription_.add_options()
		("address", boost::program_options::value<std::string>()->default_value("0.0.0.0"),
			"Set server address (default 0.0.0.0)")
		("port", boost::program_options::value<std::string>()->default_value("3000"),
			"Set port number (default 3000)")
		("threads", boost::program_options::value<std::string>()->default_value("1"),
			"Specify number of threads (default 1)")
		("root", boost::program_options::value<std::string>()->default_value("/home/romang/psi-toolkit/server/website"),
			"Set root of website files");

	optionsDescription_.add_options()
		("help", "Produce help message")
		("version", "Show version")
		("verbose", "Run verbosely");	
}

int ServerRunner::run() {

	if (executeOptions() == 1) return 1;

	try {
		PsiServer psiServer(
			options_["address"].as<std::string>(), 
			options_["port"].as<std::string>(), 
			options_["threads"].as<std::string>(), 
			options_["root"].as<std::string>()
		);

		std::cout << psiServer.info();

		// register all websites
		IndexSite index(psiServer);
		PipeSite pipe(psiServer);

		psiServer.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

int ServerRunner::executeOptions() {
	
	if (options_.count("help")) {
		std::cout << optionsDescription_ << std::endl;
		return 1;
	}

	if (options_.count("version")) {
		std::cout << "PsiServer version 0.1" << std::endl;
		return 1;
	}
	
	return 0;
}
 

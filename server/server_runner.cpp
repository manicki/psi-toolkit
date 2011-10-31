#include <unistd.h>

#include <boost/algorithm/string/join.hpp>

#include "server_runner.hpp"

#include "config.h"
#include "logging.hpp"

ServerRunner::ServerRunner(int argc, char * argv[])
    : optionsDescription("PsiServer options"){

    options = parseOptions(argc, argv);
}

boost::program_options::variables_map ServerRunner::parseOptions(int argc, char * argv[]) {

    setOptionsDescription();

    boost::program_options::variables_map vmap;
    boost::program_options::parsed_options opts =
        boost::program_options::command_line_parser(argc, argv).
        options(optionsDescription).
        allow_unregistered().
        run();

    boost::program_options::store(opts, vmap);
    boost::program_options::notify(vmap);

    // rest options needed for the further annotators
    annotatorOptions = boost::program_options::collect_unrecognized(
        opts.options, boost::program_options::include_positional);

    return vmap;
}

void ServerRunner::setOptionsDescription() {

    optionsDescription.add_options()
        ("address", boost::program_options::value<std::string>()->default_value("0.0.0.0"),
            "Set server address")
        ("port", boost::program_options::value<std::string>()->default_value("3000"),
            "Set port number")
        ("threads", boost::program_options::value<std::string>()->default_value("1"),
            "Specify number of threads")
        ("root", boost::program_options::value<std::string>()->default_value(ROOT_DIR "server/website"),
            "Set root of website files");

    optionsDescription.add_options()
        ("daemon", "Run as a daemon")
        ("leave-standard-descriptors-when-daemonizing", "Don't redirect standard input, standard output and standard error to /dev/null when daemonizing")
        ("help", "Produce help message")
        ("version", "Show version")
        ("verbose", "Run verbosely");
}

const std::string ServerRunner::DEFAULT_PIPE = "txt-reader ! tp-tokenizer --lang pl ! psi-writer";

int ServerRunner::run() {

    if (executeOptions() == 1) return 1;

    try {
        PsiServer psiServer(
            options["address"].as<std::string>(),
            options["port"].as<std::string>(),
            options["threads"].as<std::string>(),
            rootDir_.native()
        );

        std::cout << psiServer.info();

        // register all websites
        IndexSite index(psiServer);

        std::string opts = annotatorOptions.empty() ? DEFAULT_PIPE : annotatorOptionsAsString();
        PipeSite pipe(psiServer, opts);

        // run server
        psiServer.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

int ServerRunner::executeOptions() {
    if (options.count("help")) {
        std::cout << optionsDescription << std::endl;
        return 1;
    }

    if (options.count("version")) {
        std::cout << "PsiServer version 0.1" << std::endl;
        return 1;
    }

    if (setRootDirectory_() != 0)
        return 1;

    if (options.count("daemon"))
        daemonize_(options.count("leave-standard-descriptors-when-daemonizing") > 0);

    return 0;
}

int ServerRunner::setRootDirectory_() {
    boost::filesystem::path rootAsGiven(options["root"].as<std::string>());

    rootDir_ =
        // A daemon changes its current directory, so an absolute path
        // must be specified.
        (options.count("daemon")
         ? boost::filesystem::absolute(rootAsGiven)
         : rootAsGiven);

    boost::filesystem::path p(rootDir_ / "index.html");

    if (!boost::filesystem::exists(p)) {
        std::cerr << "Set path to website root directory "
                  << rootDir_
                  << " does not contain the index.html file. "
                  << "Use the --root option to specify valid root path. " << std::endl;
        return 1;
    }

    return 0;
}

void ServerRunner::daemonize_(bool leaveStandardDescriptors) {
    if (daemon(0, leaveStandardDescriptors) != 0) {
        char* errorMessage = strerror(errno);
        std::cerr << "cannot daemonize: " << errorMessage << std::endl;
    }
}

std::string ServerRunner::annotatorOptionsAsString() {
    return boost::algorithm::join(annotatorOptions, " ");
}




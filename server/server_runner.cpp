#include <unistd.h>

#include <boost/algorithm/string/join.hpp>

#include "server_runner.hpp"
#include "main_factories_keeper.hpp"

#include "console_help_formatter.hpp"

#include "config.hpp"
#include "logging.hpp"
#include "configurator.hpp"
#include "version_information.hpp"

#include "site/index_site.hpp"
#include "site/pipe_site.hpp"
#include "site/help_site.hpp"
#include "site/json_site.hpp"

ServerRunner::ServerRunner(int argc, char * argv[])
    : optionsDescription_(
    "PSIServer options"
){
    options_ = parseOptions_(argc, argv);
}

boost::program_options::variables_map ServerRunner::parseOptions_(int argc, char * argv[]) {

    setOptionsDescription_();

    boost::program_options::variables_map vmap;
    boost::program_options::parsed_options opts =
        boost::program_options::command_line_parser(argc, argv).
        options(optionsDescription_).
        allow_unregistered().
        run();

    boost::program_options::store(opts, vmap);
    boost::program_options::notify(vmap);

    // rest options needed for the further annotators
    annotatorOptions_ = boost::program_options::collect_unrecognized(
        opts.options, boost::program_options::include_positional);

    return vmap;
}

void ServerRunner::setOptionsDescription_() {

    optionsDescription_.add_options()
        ("address", boost::program_options::value<std::string>()->default_value("0.0.0.0"),
            "Set server address")
        ("port", boost::program_options::value<std::string>()->default_value("3000"),
            "Set port number")
        ("threads", boost::program_options::value<std::string>()->default_value("1"),
            "Specify number of threads")
        ("root", boost::program_options::value<std::string>()->default_value(
            (boost::filesystem::path(WEBSITE_ROOT)).string()),
            "Set root of website files");

    optionsDescription_.add_options()
        ("daemon", "run as a daemon")
        ("leave-standard-descriptors-when-daemonizing", "don't redirect standard input, "
            "standard output and standard error to /dev/null when daemonizing")
        ("help", "produce help message for each processor")
        ("version", "show version information and exit")
        ("verbose", "run verbosely")
        ("log-level", boost::program_options::value<std::string>(), "set logging level")
        ("log-file", boost::program_options::value<std::string>(),
            "filepath to store logs (if not set: standard error)")
        ;
}

const std::string ServerRunner::DEFAULT_PIPE = "txt-reader ! tp-tokenizer --lang pl ! psi-writer";

int ServerRunner::run() {

    if (stopAfterExecutingOptions_()) return 0;

    try {
        PsiServer psiServer(
            options_["address"].as<std::string>(),
            options_["port"].as<std::string>(),
            options_["threads"].as<std::string>(),
            rootDir_.string()
        );

        std::cout << psiServer.info();

        // register all websites
        IndexSite index(psiServer);
        std::string opts = annotatorOptions_.empty() ? DEFAULT_PIPE : annotatorOptionsAsString_();
        PipeSite pipe(psiServer, opts);
        HelpSite help(psiServer);
        JsonSite json(psiServer);

        // run server
        psiServer.run();
    }
    catch (std::exception& e) {
        ERROR("Exception: " << e.what());
    }

    return 0;
}

bool ServerRunner::stopAfterExecutingOptions_() {
    if (options_.count("help")) {
        std::cout << optionsDescription_ << std::endl;
        ConsoleHelpFormatter().formatHelps(std::cout);
        return true;
    }

    if (options_.count("version")) {
        std::cout << get_psi_toolkit_version_string() << std::endl;
        return true;
    }

    if (setRootDirectory_() != 0)
        return true;

    if (options_.count("log-file")) {
        SET_LOGGER_FILE(options_["log-file"].as<std::string>());
    }

    if (options_.count("log-level")) {
        SET_LOGGING_LEVEL(options_["log-level"].as<std::string>());
    }


    if (options_.count("daemon"))
        daemonize_(options_.count("leave-standard-descriptors-when-daemonizing") > 0);

    return false;
}

int ServerRunner::setRootDirectory_() {
    boost::filesystem::path rootAsGiven(options_["root"].as<std::string>());

    rootDir_ =
        // A daemon changes its current directory, so an absolute path
        // must be specified.
        (options_.count("daemon") && !rootAsGiven.has_root_directory()
         ? boost::filesystem::current_path() / rootAsGiven
         : rootAsGiven);

    boost::filesystem::path p(rootDir_ / "index.html");

    if (!boost::filesystem::exists(p)) {
        ERROR("Set path to website root directory " << rootDir_.string()
              << " does not contain the index.html file. \n"
              << "Use the --root option to specify valid root path. "
              );
        return 1;
    }

    return 0;
}

void ServerRunner::daemonize_(bool leaveStandardDescriptors) {
    if (daemon(0, leaveStandardDescriptors) != 0) {
        char* errorMessage = strerror(errno);
        ERROR("cannot daemonize: " << errorMessage);
    }
}

std::string ServerRunner::annotatorOptionsAsString_() {
    return boost::algorithm::join(annotatorOptions_, " ");
}

#include "mass_tester.hpp"

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/foreach.hpp>

#include "logging.hpp"

MassTester::MassTester(int argc, char* argv[]) {
    boost::program_options::options_description optionsDescription = optionsHandled();

    boost::program_options::positional_options_description positionalDescription;
    positionalDescription.add("directory", -1);

    boost::program_options::variables_map options;


    boost::program_options::command_line_parser parser(argc, argv);
    parser.options(optionsDescription).positional(positionalDescription);

    boost::program_options::store(parser.run(), options);
    boost::program_options::notify(options);

    std::vector<std::string> directories = options["directory"].as<std::vector<std::string> >();

    BOOST_FOREACH(std::string directory, directories) {
        directories_.push_back(directory);
    }
}

int MassTester::run() {
    BOOST_FOREACH(boost::filesystem::path path, directories_) {
        INFO("will look in " << path);
    }

    return 0;
}

boost::program_options::options_description MassTester::optionsHandled() {

    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("directory", boost::program_options::value<std::vector<std::string> >(),
         "directories to traverse")
        ;

    return optionsDescription;
}



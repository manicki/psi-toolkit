#include "tests.hpp"

#include "config.hpp"
#include "bilexicon.hpp"

#include <boost/program_options/parsers.hpp>

BOOST_AUTO_TEST_SUITE( bilexicon )

BOOST_AUTO_TEST_CASE( bilexicon_simple ) {
    const char* argv[3] = {
        "--plain-text-format",
        ROOT_DIR "tools/lexica/bilexicon/t/plen.txt",
        0};

    const int argc = 2;

    boost::program_options::variables_map options;
    boost::program_options::store(
        boost::program_options::parse_command_line(
            argc, argv, BiLexicon::optionsHandled()), options);
    boost::program_options::notify(options);

    BiLexicon biLexicon(options);
}

BOOST_AUTO_TEST_SUITE_END()


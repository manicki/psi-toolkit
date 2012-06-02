#include "tests.hpp"

#include <boost/program_options/parsers.hpp>

#include "sfst_lemmatizer.hpp"
#include "by_spaces_cutter.hpp"

#include "lemmatizer_annotator.hpp"

BOOST_AUTO_TEST_SUITE( sfst )

BOOST_AUTO_TEST_CASE( simple_stem ) {
    int argc = 3;
    const char* argv[4] = { "", "--lang", "tr", 0};

    boost::program_options::variables_map options;
    boost::program_options::store(
        boost::program_options::parse_command_line(
            argc, argv,
            SfstLemmatizer::optionsHandled()), options);
    boost::program_options::notify(options);

    SfstLemmatizer morf(options);

    std::vector<std::string> stems = morf.simpleStem("gelmedik");
    BOOST_CHECK_EQUAL((int)stems.size(), 1);

    std::vector<std::string>::iterator s = stems.begin();
    BOOST_CHECK_EQUAL(*s, "gelmek");
}

BOOST_AUTO_TEST_SUITE_END()

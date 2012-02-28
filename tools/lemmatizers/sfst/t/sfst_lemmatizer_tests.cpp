#include "tests.hpp"

#include "sfst_lemmatizer.hpp"
#include "by_spaces_cutter.hpp"

#include "lemmatizer_annotator.hpp"

BOOST_AUTO_TEST_SUITE( sfst )

BOOST_AUTO_TEST_CASE( simple_stem ) {
    boost::program_options::variables_map noOptions;
    sfstLemmatizer morf(noOptions);

    std::vector<std::string> stems = morf.simpleStem("gelmedik");
    BOOST_CHECK_EQUAL((int)stems.size(), 1);

    std::vector<std::string>::iterator s = stems.begin();
    BOOST_CHECK_EQUAL(*s, "gel");
}

BOOST_AUTO_TEST_SUITE_END()

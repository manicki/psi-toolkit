#include "tests.hpp"

#include "regexp.hpp"

BOOST_AUTO_TEST_SUITE( regexp )

BOOST_AUTO_TEST_CASE( utf8 ) {
    RegExp re("(ż+)");

    std::string arg;
    BOOST_CHECK(RegExp::FullMatch("żżż", re, &arg));
    BOOST_CHECK_EQUAL(arg, "żżż");

    PerlRegExp perlRe("(ż+)");

    std::string perlArg;
    BOOST_CHECK(PerlRegExp::FullMatch("żżż", perlRe, &perlArg));
    BOOST_CHECK_EQUAL(perlArg, "żżż");
}

BOOST_AUTO_TEST_SUITE_END()

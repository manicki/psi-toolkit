#include "tests.hpp"

#include "../string_frag.hpp"

BOOST_AUTO_TEST_CASE( string_frag_simple ) {
    std::string source("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    StringFrag sf1(source.substr(0, 10));
    StringFrag sf2(source, 0, 10);
    BOOST_CHECK_EQUAL(sf1.str(), sf2.str());
    StringFrag sf3(source.substr(10, 10));
    StringFrag sf4(source, 10, 10);
    sf1.append(sf4);
    sf2.append(sf4);
    BOOST_CHECK_EQUAL(sf1.str(), sf2.str());
    sf1.append(sf3);
    sf2.append(sf3);
    BOOST_CHECK_EQUAL(sf1.str(), sf2.str());
};

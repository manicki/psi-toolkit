#include "tests.hpp"

#include "string_frag.hpp"

BOOST_AUTO_TEST_CASE( string_frag_simple ) {
    std::string source("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    std::string sub1 = source.substr(0, 10);
    StringFrag sf1(sub1);
    BOOST_CHECK_EQUAL(sf1[5], sub1[5]);
    BOOST_CHECK_EQUAL(sf1.data(), sub1.data());
    StringFrag sf2(source, 0, 10);
    BOOST_CHECK_EQUAL(sf1.str(), sf2.str());
    std::string sub2 = source.substr(10, 10);
    StringFrag sf3(sub2);
    StringFrag sf4(source, 10, 10);
    sf1.append(sf4);
    sf2.append(sf4);
    BOOST_CHECK_EQUAL(sf1.str(), sf2.str());
    sf1.append(sf3);
    sf2.append(sf3);
    BOOST_CHECK_EQUAL(sf1.str(), sf2.str());
}

BOOST_AUTO_TEST_CASE( string_frag_frag ) {
    std::string source("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    StringFrag sf(source, 2, 16);
    StringFrag sf1(sf, 4, 8);
    std::string sub = source.substr(2, 16);
    StringFrag sf2(sub, 4, 8);
    BOOST_CHECK_EQUAL(sf1.str(), sf2.str());
}

BOOST_AUTO_TEST_CASE( string_frag_find ) {
    std::string source("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    std::string ss = source.substr(2, 16);
    StringFrag sf(source, 2, 16);
    BOOST_CHECK_EQUAL(sf.find(' '), ss.find(' '));
    BOOST_CHECK_EQUAL(sf.find(' ', 5), ss.find(' ', 5));
}

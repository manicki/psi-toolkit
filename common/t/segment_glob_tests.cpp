#include "segment_glob.hpp"

#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( simple ) {
    SegmentGlob glob("*.txt");

    BOOST_CHECK(!glob.isFixed());
    BOOST_CHECK(glob.matches("file.txt"));
    BOOST_CHECK(glob.matches(".txt"));
    BOOST_CHECK(glob.matches("ź.txt"));
    BOOST_CHECK(!glob.matches("a-txt"));
    BOOST_CHECK(!glob.matches("file.txt.bak"));
    BOOST_CHECK(!glob.matches("txt.file"));
    BOOST_CHECK(!glob.matches(".tx"));
    BOOST_CHECK(!glob.matches(""));
    BOOST_CHECK(!glob.matches("txt"));

    BOOST_CHECK_EQUAL(glob.asString(), "*.txt");
}

BOOST_AUTO_TEST_CASE( question_mark ) {

    SegmentGlob glob("file?.txt");

    BOOST_CHECK(!glob.isFixed());
    BOOST_CHECK(glob.matches("filex.txt"));
    BOOST_CHECK(glob.matches("file1.txt"));
    BOOST_CHECK(!glob.matches("file.txt"));
    BOOST_CHECK(glob.matches("file?.txt"));
    BOOST_CHECK(!glob.matches(".txt"));
}

BOOST_AUTO_TEST_CASE( question_mark_extended ) {

    SegmentGlob glob("file?.tx?");

    BOOST_CHECK(!glob.isFixed());
    BOOST_CHECK(glob.matches("filex.txt"));
    BOOST_CHECK(glob.matches("filex.txb"));
    BOOST_CHECK(!glob.matches("filex.txbb"));
    BOOST_CHECK(glob.matches("file1.txt"));
    BOOST_CHECK(!glob.matches("file.txt"));
    BOOST_CHECK(glob.matches("file?.txt"));
    BOOST_CHECK(!glob.matches(".txt"));
}

BOOST_AUTO_TEST_CASE( fixed ) {

    SegmentGlob glob("test.gif");

    BOOST_CHECK(glob.isFixed());
    BOOST_CHECK(glob.matches("test.gif"));
    BOOST_CHECK(!glob.matches("test"));
    BOOST_CHECK(!glob.matches("bla-test.gif"));
    BOOST_CHECK(!glob.matches(""));
}

BOOST_AUTO_TEST_CASE( hard ) {

    SegmentGlob glob("ab*ab*b*c?");

    BOOST_CHECK(!glob.isFixed());

    BOOST_CHECK(glob.matches("ababbcx"));
    BOOST_CHECK(glob.matches("abbbbbabbbbbbbbcccc"));
    BOOST_CHECK(glob.matches("abbbbbabbbbbbbbccccd"));
    BOOST_CHECK(glob.matches("ababbbbbc0"));
    BOOST_CHECK(!glob.matches("abbbbbbcd"));
    BOOST_CHECK(!glob.matches(""));
}

BOOST_AUTO_TEST_CASE( utf8 ) {
    SegmentGlob glob("a??.txt");

    BOOST_CHECK(!glob.isFixed());

    BOOST_CHECK(glob.matches("abc.txt"));
    BOOST_CHECK(glob.matches("aąć.txt"));
    BOOST_CHECK(glob.matches("aąc.txt"));
    BOOST_CHECK(!glob.matches("aą.txt"));

    SegmentGlob globB("a*b??.txt");

    BOOST_CHECK(globB.matches("abźż.txt"));
    BOOST_CHECK(globB.matches("abćą.txt"));
    BOOST_CHECK(!globB.matches("abć.txt"));
}

BOOST_AUTO_TEST_CASE( all ) {

    SegmentGlob glob("*");

    BOOST_CHECK(!glob.isFixed());

    BOOST_CHECK(glob.matches("a.txt"));
    BOOST_CHECK(glob.matches(""));
    BOOST_CHECK(glob.matches("a"));
}


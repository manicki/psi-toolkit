#include "tests.hpp"

#include "aliaser.hpp"
#include <boost/assign/list_of.hpp>

BOOST_AUTO_TEST_SUITE( aliaser )

BOOST_AUTO_TEST_CASE( simple ) {
    Aliaser aliaser;

    aliaser.addVoidAlias("baq");
    aliaser.addAlias("foo", "bar");
    aliaser.addAlias("baz", "CCC");
    aliaser.addAlias("baz", "BBB");
    aliaser.addAlias("bar", "BBB");
    aliaser.addAlias("bar", "AAA");
    aliaser.addAlias("bar", "baz");
    aliaser.addAlias("bar", "baq");
    aliaser.addAlias("foo", "AAA");

    {
        std::set<std::string> expectedSet =
            boost::assign::list_of
            (std::string("AAA"))
            (std::string("BBB"))
            (std::string("CCC"));

        BOOST_CHECK(
            aliaser.getAllDestinations("foo") ==
            expectedSet);
    }

    {
        std::set<std::string> expectedSet =
            boost::assign::list_of
            (std::string("BBB"))
            (std::string("CCC"));

        BOOST_CHECK(
            aliaser.getAllDestinations("baz") ==
            expectedSet);
    }

    {
        std::set<std::string> expectedSet =
            boost::assign::list_of
            (std::string("foo"))
            (std::string("bar"))
            (std::string("baz"));

        BOOST_CHECK(
            aliaser.getAllAliases("CCC") ==
            expectedSet);
    }

    {
        std::set<std::string> expectedSet =
            boost::assign::list_of
            (std::string("foo"))
            (std::string("bar"))
            (std::string("baq"))
            (std::string("baz"));

        BOOST_CHECK(
            aliaser.getAliasNames() ==
            expectedSet);
    }

    BOOST_CHECK(aliaser.getAllDestinations("baq").empty());
    BOOST_CHECK(aliaser.getAllAliases("zzz").empty());
    BOOST_CHECK(aliaser.isAlias("baq"));
    BOOST_CHECK(aliaser.isAlias("foo"));
    BOOST_CHECK(!aliaser.isAlias("AAA"));
    BOOST_CHECK(!aliaser.isAlias("zzz"));

}


BOOST_AUTO_TEST_SUITE_END()

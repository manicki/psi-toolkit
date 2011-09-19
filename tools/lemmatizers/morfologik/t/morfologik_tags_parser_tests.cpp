#include "tests.hpp"

#include "../morfologik_tags_parser.hpp"

BOOST_AUTO_TEST_SUITE( morfologik_tags_parser )

BOOST_AUTO_TEST_CASE( simple_tag ) {
    MorfologikTagsParser mrfTagPrs;
    std::string tag = "subst:sg:inst:n"; // For word "dzieckiem"
    std::vector<std::map<std::string, std::string> > result =
        mrfTagPrs.parse(tag);

    BOOST_REQUIRE_EQUAL((int)result.size(), 1);
    BOOST_CHECK_EQUAL((int)result[0].size(), 4);
}

BOOST_AUTO_TEST_CASE ( complex_tag ) {
    MorfologikTagsParser mrfTagPrs;
    std::string tag = "adj:pl:acc.nom.voc:f.m2.m3.n:pos"; // For word "jakie"
    std::vector<std::map<std::string, std::string> > result =
        mrfTagPrs.parse(tag);

    BOOST_CHECK_EQUAL((int)result.size(), 12);
}

BOOST_AUTO_TEST_SUITE_END()

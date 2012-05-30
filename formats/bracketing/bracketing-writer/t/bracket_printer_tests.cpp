#include "tests.hpp"

#include <set>
#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/foreach.hpp>

#include "bracket_printer.hpp"
#include "edge_data.hpp"


void testBracketPrinter(std::string pattern, std::string output) {
    std::vector<std::string> vs = boost::assign::list_of(pattern);
    BracketPrinter bp(vs, ",", ",", "=");
    std::set<std::string> tags = boost::assign::list_of("symbol")("token")("segment");
    std::map<std::string, std::string> avMap
        = boost::assign::map_list_of("case", "Nominative")("number", "singular");
    EdgeData edgeData(tags, "Noun-Phrase", "Żółta jaźń", avMap, -1.5);
    std::set<EdgeData> edgeDataSet;
    edgeDataSet.insert(edgeData);
    std::set< std::vector<std::string> > printed = bp.print(edgeDataSet);
    BOOST_FOREACH(std::vector<std::string> p, printed) {
        BOOST_CHECK_EQUAL(p[0], output);
    }
}


BOOST_AUTO_TEST_SUITE( bracket_printer )


BOOST_AUTO_TEST_CASE( bracket_printer_simple ) {
    testBracketPrinter(
        "<%c tags=\"%T\" score=\"%s\">%t</%c> % %",
        "<Noun-Phrase tags=\"segment,symbol,token\" score=\"-1.5\">Żółta jaźń</Noun-Phrase> % %"
    );
    testBracketPrinter(
        "<%c attr=\"%A\">%t</%c> %\n",
        "<Noun-Phrase attr=\"case=Nominative,number=singular\">Żółta jaźń</Noun-Phrase> %\n"
    );
    testBracketPrinter(
        "<%c case=%a[case] number=%a`number`>%t</%c %a> %a",
        "<Noun-Phrase case=Nominative number=singular>Żółta jaźń</Noun-Phrase %a> %a"
    );
}


BOOST_AUTO_TEST_SUITE_END()

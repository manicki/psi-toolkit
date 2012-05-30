#include "tests.hpp"

#include <set>
#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/foreach.hpp>

#include "bracket_printer.hpp"
#include "edge_data.hpp"


BOOST_AUTO_TEST_SUITE( bracket_printer )


BOOST_AUTO_TEST_CASE( bracket_printer_simple ) {
    std::vector<std::string> vs1
        = boost::assign::list_of("<%c tags=\"%T\" score=\"%s\">%t</%c> % %");
    std::vector<std::string> vs2
        = boost::assign::list_of("<%c attr=\"%A\">%t</%c> %\n");
    std::vector<std::string> vs3
        = boost::assign::list_of("<%c case=%a[case] number=%a`number`>%t</%c %a> %a");
    BracketPrinter bp1(vs1, ",", ",", "=");
    BracketPrinter bp2(vs2, ",", ",", "=");
    BracketPrinter bp3(vs3, ",", ",", "=");
    std::set<std::string> tags = boost::assign::list_of("symbol")("token")("segment");
    std::map<std::string, std::string> avMap
        = boost::assign::map_list_of("case", "Nominative")("number", "singular");
    EdgeData edgeData(tags, "Noun-Phrase", "Żółta jaźń", avMap, -1.5);
    std::set<EdgeData> edgeDataSet;
    edgeDataSet.insert(edgeData);
    std::set< std::vector<std::string> > printed1 = bp1.print(edgeDataSet);
    BOOST_FOREACH(std::vector<std::string> p, printed1) {
        BOOST_CHECK_EQUAL(
            p[0],
            "<Noun-Phrase tags=\"segment,symbol,token\" score=\"-1.5\">Żółta jaźń</Noun-Phrase> % %"
        );
    }
    std::set< std::vector<std::string> > printed2 = bp2.print(edgeDataSet);
    BOOST_FOREACH(std::vector<std::string> p, printed2) {
        BOOST_CHECK_EQUAL(
            p[0],
            "<Noun-Phrase attr=\"case=Nominative,number=singular\">Żółta jaźń</Noun-Phrase> %\n"
        );
    }
    std::set< std::vector<std::string> > printed3 = bp3.print(edgeDataSet);
    BOOST_FOREACH(std::vector<std::string> p, printed3) {
        BOOST_CHECK_EQUAL(
            p[0],
            "<Noun-Phrase case=Nominative number=singular>Żółta jaźń</Noun-Phrase %a> %a"
        );
    }
}


BOOST_AUTO_TEST_SUITE_END()

#include "tests.hpp"

#include <boost/assign.hpp>

#include "active_elements_printer.hpp"


BOOST_AUTO_TEST_SUITE( active_elements_printer )


BOOST_AUTO_TEST_CASE( active_elements_printer_simple ) {
    ActiveElementsPrinter::Manager aepManager(",", ",", "=");
    ActiveElementsPrinter aep1
        = aepManager.getPrinter("<%c tags=\"%T\" score=\"%s\">%t</%c> % %");
    ActiveElementsPrinter aep2
        = aepManager.getPrinter("<%c attr=\"%A\">%t</%c> %\n");
    ActiveElementsPrinter aep3
        = aepManager.getPrinter("<%c case=%a[case] number=%a`number`>%t</%c %a> %a");
    std::set<std::string> tags = boost::assign::list_of("symbol")("token")("segment");
    std::map<std::string, std::string> avMap
        = boost::assign::map_list_of("case", "Nominative")("number", "singular");
    EdgeData edgeData(tags, "Noun-Phrase", "Żółta jaźń", avMap, -1.5);
    BOOST_CHECK_EQUAL(
        aep1.print(edgeData),
        "<Noun-Phrase tags=\"segment,symbol,token\" score=\"-1.5\">Żółta jaźń</Noun-Phrase> % %"
    );
    BOOST_CHECK_EQUAL(
        aep1.print(tags, "Noun-Phrase", "Żółta jaźń", avMap, -1.5),
        "<Noun-Phrase tags=\"segment,symbol,token\" score=\"-1.5\">Żółta jaźń</Noun-Phrase> % %"
    );
    BOOST_CHECK_EQUAL(
        aep2.print(tags, "Noun-Phrase", "Żółta jaźń", avMap, -1.5),
        "<Noun-Phrase attr=\"case=Nominative,number=singular\">Żółta jaźń</Noun-Phrase> %\n"
    );
    BOOST_CHECK_EQUAL(
        aep3.print(tags, "Noun-Phrase", "Żółta jaźń", avMap, -1.5),
        "<Noun-Phrase case=Nominative number=singular>Żółta jaźń</Noun-Phrase %a> %a"
    );
}


BOOST_AUTO_TEST_SUITE_END()

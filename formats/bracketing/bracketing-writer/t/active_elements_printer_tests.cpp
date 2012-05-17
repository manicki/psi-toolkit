#include "tests.hpp"

#include <boost/assign.hpp>

#include "active_elements_printer.hpp"


BOOST_AUTO_TEST_SUITE( active_elements_printer )


BOOST_AUTO_TEST_CASE( active_elements_printer_simple ) {
    ActiveElementsPrinter::Manager aepManager(",");
    ActiveElementsPrinter aep = aepManager.getPrinter("<%c tags=\"%T\" score=\"%s\">%t</%c> % %");
    std::list<std::string> tags = boost::assign::list_of("symbol")("token")("segment");
    BOOST_CHECK_EQUAL(
        aep.print(tags, "Noun-Phrase", "Żółta jaźń", -1.5),
        "<Noun-Phrase tags=\"symbol,token,segment\" score=\"-1.5\">Żółta jaźń</Noun-Phrase> % %"
    );
}


BOOST_AUTO_TEST_SUITE_END()

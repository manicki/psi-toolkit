#include <boost/foreach.hpp>

#include "tests.hpp"
#include "config.hpp"

#include "format_specification.hpp"

BOOST_AUTO_TEST_SUITE( format_specification )

BOOST_AUTO_TEST_CASE( format_specification_xml_reading ) {

    FormatSpecificationReader reader(ROOT_DIR "formats/apertium/t/files/simple-format.xml");
    FormatSpecification specification = reader.readFormatSpecification();

    BOOST_CHECK_EQUAL(specification.formatRuleSize(), 3);

    // string like this one is expected: "((?:\\s*?)abc(?:(?:\\s|.)*?)cba(?:\t*))" "|"
    // "((?:\\s*?)t(?:\t*))" "|" "((?:\\s*?)(?:t1)|(?:t2)(?:\t*))";
    std::vector<std::string> expectedInRegexp;
    expectedInRegexp.push_back(")abc(");
    expectedInRegexp.push_back(")cba(");
    expectedInRegexp.push_back("(?:t1)|(?:t2)");
    expectedInRegexp.push_back(")|(");

    BOOST_FOREACH(std::string str, expectedInRegexp) {
        BOOST_CHECK(specification.formatRulesRegexp().find(str) != std::string::npos);
    }

    std::map<std::string, std::string> replacements = specification.replacementRulesRegexp();
    BOOST_CHECK_EQUAL(replacements.size(), 3);

    std::map<std::string, std::string>::iterator it = replacements.find("1");
    BOOST_CHECK(it != replacements.end());
    BOOST_CHECK((*it).second.find("a") != std::string::npos);
    BOOST_CHECK((*it).second.find("A") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()

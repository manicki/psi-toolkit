#include "tests.hpp"

#include "srx_rules.hpp"
#include "config.hpp"

#include <vector>

BOOST_AUTO_TEST_SUITE( srx_rules )

BOOST_AUTO_TEST_CASE( srx_simple ) {

    std::vector<SrxRule> srxRules;

    SrxRulesReader rulesReader(
        ROOT_DIR "tools/segmenters/srx/t/simple.srx",
        "pl");

    rulesReader.getRules(std::back_inserter(srxRules));

    BOOST_CHECK_EQUAL(srxRules.size(), 4U);

    BOOST_CHECK_EQUAL(srxRules[0].isBreakable(), false);
    BOOST_CHECK_EQUAL(srxRules[0].getBeforeBreak(), "^\\s*[0-9]+\\.");
    BOOST_CHECK_EQUAL(srxRules[0].getAfterBreak(), "\\s");

    BOOST_CHECK_EQUAL(srxRules[1].isBreakable(), false);
    BOOST_CHECK_EQUAL(srxRules[1].getBeforeBreak(), "[Nn][Pp]\\.");
    BOOST_CHECK_EQUAL(srxRules[1].getAfterBreak(), "");

    BOOST_CHECK_EQUAL(srxRules[2].isBreakable(), true);
    BOOST_CHECK_EQUAL(srxRules[2].getBeforeBreak(), "[\\.\\?!]+");
    BOOST_CHECK_EQUAL(srxRules[2].getAfterBreak(), "\\s");

    BOOST_CHECK_EQUAL(srxRules[3].isBreakable(), true);
    BOOST_CHECK_EQUAL(srxRules[3].getBeforeBreak(), "");
    BOOST_CHECK_EQUAL(srxRules[3].getAfterBreak(), "\\n");
}

BOOST_AUTO_TEST_CASE( real_one ) {

    std::vector<SrxRule> srxRules;

    SrxRulesReader rulesReader(
        ROOT_DIR "tools/segmenters/srx/data/pl/segmentation.srx",
        "pl");

    rulesReader.getRules(std::back_inserter(srxRules));

    BOOST_CHECK(1U < srxRules.size());
}

BOOST_AUTO_TEST_SUITE_END()

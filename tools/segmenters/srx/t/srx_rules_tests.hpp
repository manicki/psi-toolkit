#include "../srx_rules.hpp"

#include "config.h"

#include <vector>

class SrxRulesTests : public CxxTest::TestSuite {

public:
    void testSimple() {
        std::vector<SrxRule> srxRules;

        SrxRulesReader rulesReader(
            ROOT_DIR "tools/segmenters/srx/t/simple.srx",
            "pl");

        rulesReader.getRules(std::back_inserter(srxRules));

        TS_ASSERT_EQUALS(srxRules.size(), 4U);

        TS_ASSERT_EQUALS(srxRules[0].isBreakable(), false);
        TS_ASSERT_EQUALS(srxRules[0].getBeforeBreak(), "^\\s*[0-9]+\\.");
        TS_ASSERT_EQUALS(srxRules[0].getAfterBreak(), "\\s");

        TS_ASSERT_EQUALS(srxRules[1].isBreakable(), false);
        TS_ASSERT_EQUALS(srxRules[1].getBeforeBreak(), "[Nn][Pp]\\.");
        TS_ASSERT_EQUALS(srxRules[1].getAfterBreak(), "");

        TS_ASSERT_EQUALS(srxRules[2].isBreakable(), true);
        TS_ASSERT_EQUALS(srxRules[2].getBeforeBreak(), "[\\.\\?!]+");
        TS_ASSERT_EQUALS(srxRules[2].getAfterBreak(), "\\s");

        TS_ASSERT_EQUALS(srxRules[3].isBreakable(), true);
        TS_ASSERT_EQUALS(srxRules[3].getBeforeBreak(), "");
        TS_ASSERT_EQUALS(srxRules[3].getAfterBreak(), "\\n");
    }
};



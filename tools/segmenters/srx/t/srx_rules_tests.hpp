#include "../srx_rules.hpp"

#include "config.h"

class SrxRulesTests : public CxxTest::TestSuite {

public:
    void testSimple() {
        SrxRulesReader rulesReader(
            ROOT_DIR "tools/segmenters/srx/t/simple.srx",
            "pl");
    }
};



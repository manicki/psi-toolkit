#include "regexp.hpp"

class RegexpTests : public CxxTest::TestSuite
{
public:
    void test_utf8() {
        RegExp re("(ż+)");

        std::string arg;
        TS_ASSERT(RegExp::FullMatch("żżż", re, &arg));
        TS_ASSERT_EQUALS(arg, "żżż");
    }

};


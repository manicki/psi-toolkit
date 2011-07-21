#include "psi_quoter.hpp"
#include "utt_quoter.hpp"


class QuoterTests : public CxxTest::TestSuite {

public:

    void test_psi_quoter() {
        Quoter * quoter = new PsiQuoter();
        TS_ASSERT_EQUALS(quoter->escape("#"), "\\#");
        TS_ASSERT_EQUALS(quoter->unescape("\\#"), "#");
        TS_ASSERT_EQUALS(quoter->escape("a ą\\b_c\nć\td#e"), "a_ą\\\\b\\_c\\nć\\td\\#e");
        TS_ASSERT_EQUALS(quoter->unescape("a_ą\\\\b\\_c\\nć\\td\\#e"), "a ą\\b_c\nć\td#e");
        delete quoter;
    }

    void test_utt_quoter() {
        Quoter * quoter = new UTTQuoter();
        TS_ASSERT_EQUALS(quoter->escape("#"), "#");
        TS_ASSERT_EQUALS(quoter->unescape("#"), "#");
        TS_ASSERT_EQUALS(quoter->escape("a ą\\b_c\nć\td#e"), "a_ą\\\\b\\_c\\nć\\td#e");
        TS_ASSERT_EQUALS(quoter->unescape("a_ą\\\\b\\_c\\nć\\td#e"), "a ą\\b_c\nć\td#e");
        delete quoter;
    }

};

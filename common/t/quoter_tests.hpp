#include "psi_quoter.hpp"
#include "utt_quoter.hpp"


class QuoterTests : public CxxTest::TestSuite {

public:

    void test_psi_quoter() {
        Quoter * quoter = new PsiQuoter();
        TS_ASSERT_EQUALS(quoter->escape("a"), "a");
        TS_ASSERT_EQUALS(quoter->unescape("a"), "a");
        TS_ASSERT_EQUALS(quoter->escape("ą"), "ą");
        TS_ASSERT_EQUALS(quoter->unescape("ą"), "ą");
        TS_ASSERT_EQUALS(quoter->escape(" "), "_");
        TS_ASSERT_EQUALS(quoter->unescape("_"), " ");
        TS_ASSERT_EQUALS(quoter->escape("\n"), "\\n");
        TS_ASSERT_EQUALS(quoter->unescape("\\n"), "\n");
        TS_ASSERT_EQUALS(quoter->escape("\t"), "\\t");
        TS_ASSERT_EQUALS(quoter->unescape("\\t"), "\t");
        TS_ASSERT_EQUALS(quoter->escape("\r"), "\\r");
        TS_ASSERT_EQUALS(quoter->unescape("\\r"), "\r");
        TS_ASSERT_EQUALS(quoter->escape("*"), "\\*");
        TS_ASSERT_EQUALS(quoter->unescape("\\*"), "*");
        TS_ASSERT_EQUALS(quoter->escape("\\"), "\\\\");
        TS_ASSERT_EQUALS(quoter->unescape("\\\\"), "\\");
        TS_ASSERT_EQUALS(quoter->escape("#"), "\\#");
        TS_ASSERT_EQUALS(quoter->unescape("\\#"), "#");
        TS_ASSERT_EQUALS(quoter->escape("Zażółć gęślą jaźń!"), "Zażółć_gęślą_jaźń!");
        TS_ASSERT_EQUALS(quoter->unescape("Zażółć_gęślą_jaźń!"), "Zażółć gęślą jaźń!");
        delete quoter;
    }

    void test_utt_quoter() {
        Quoter * quoter = new UTTQuoter();
        TS_ASSERT_EQUALS(quoter->escape("a"), "a");
        TS_ASSERT_EQUALS(quoter->unescape("a"), "a");
        TS_ASSERT_EQUALS(quoter->escape("ą"), "ą");
        TS_ASSERT_EQUALS(quoter->unescape("ą"), "ą");
        TS_ASSERT_EQUALS(quoter->escape(" "), "_");
        TS_ASSERT_EQUALS(quoter->unescape("_"), " ");
        TS_ASSERT_EQUALS(quoter->escape("\n"), "\\n");
        TS_ASSERT_EQUALS(quoter->unescape("\\n"), "\n");
        TS_ASSERT_EQUALS(quoter->escape("\t"), "\\t");
        TS_ASSERT_EQUALS(quoter->unescape("\\t"), "\t");
        TS_ASSERT_EQUALS(quoter->escape("\r"), "\\r");
        TS_ASSERT_EQUALS(quoter->unescape("\\r"), "\r");
        TS_ASSERT_EQUALS(quoter->escape("*"), "\\*");
        TS_ASSERT_EQUALS(quoter->unescape("\\*"), "*");
        TS_ASSERT_EQUALS(quoter->escape("\\"), "\\\\");
        TS_ASSERT_EQUALS(quoter->unescape("\\\\"), "\\");
        TS_ASSERT_EQUALS(quoter->escape("#"), "#");
        TS_ASSERT_EQUALS(quoter->unescape("#"), "#");
        TS_ASSERT_EQUALS(quoter->escape("Zażółć gęślą jaźń!"), "Zażółć_gęślą_jaźń!");
        TS_ASSERT_EQUALS(quoter->unescape("Zażółć_gęślą_jaźń!"), "Zażółć gęślą jaźń!");
        delete quoter;
    }

};

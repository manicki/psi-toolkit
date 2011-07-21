#include "psi_quoter.hpp"
#include "utt_quoter.hpp"


class QuoterTests : public CxxTest::TestSuite {

public:

    void test_psi_quoter() {
        Quoter * quoter = new PsiQuoter();
        TS_ASSERT_EQUALS(quoter->escape("Ala ma kota."), "Ala_ma_kota.");
        TS_ASSERT_EQUALS(quoter->unescape("Ala_ma_kota."), "Ala ma kota.");
        delete quoter;
    }

    void test_utt_quoter() {
        Quoter * quoter = new UTTQuoter();
        TS_ASSERT_EQUALS(quoter->escape("Ala ma kota."), "Ala_ma_kota.");
        TS_ASSERT_EQUALS(quoter->unescape("Ala_ma_kota."), "Ala ma kota.");
        delete quoter;
    }

};

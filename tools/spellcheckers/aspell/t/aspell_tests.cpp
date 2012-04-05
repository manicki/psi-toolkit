#include "tests.hpp"

#include "psi_aspell.hpp"

BOOST_AUTO_TEST_SUITE( aspell )

BOOST_AUTO_TEST_CASE( run_aspell ) {
    PSIAspell aspellAdapter(std::string("pl_PL"));

    BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_SUITE_END()

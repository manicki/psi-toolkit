#include "tests.hpp"

#include "number_master.hpp"
#include "tgbg_combinator.tpl"


BOOST_AUTO_TEST_SUITE( gobio_tgbg )


BOOST_AUTO_TEST_CASE( reading_rules ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_3.g");

}


BOOST_AUTO_TEST_CASE( reading_markov_binarized_rules ) {

    {
        tgbg_combinator<
            int,
            Lattice::Score,
            number_master,
            semantics_stub<int, number_master, double>
        > tgbg;
        tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_4.g");
    }

    {
        tgbg_combinator<
            int,
            Lattice::Score,
            number_master,
            semantics_stub<int, number_master, double>
        > tgbg;
        tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_5.g");
    }

    {
        tgbg_combinator<
            int,
            Lattice::Score,
            number_master,
            semantics_stub<int, number_master, double>
        > tgbg;
        tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_6.g");
    }

}


BOOST_AUTO_TEST_SUITE_END()

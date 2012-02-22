#include "tests.hpp"

#include "number_master.hpp"
#include "test_helpers.hpp"
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


BOOST_AUTO_TEST_CASE( compiling_binarized_rules ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_6.g");
    tgbg.compile_all_rules();

}


BOOST_AUTO_TEST_CASE( tgbg_linearization_optional ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_7.g");

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_7.g.out")
    );

}


BOOST_AUTO_TEST_CASE( tgbg_linearization ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_8.g");

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_8.g.out")
    );

}


BOOST_AUTO_TEST_SUITE_END()

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


BOOST_AUTO_TEST_CASE( tgbg_binarization_simple ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_9.g");

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_9.g.out")
    );

}

/*
BOOST_AUTO_TEST_CASE( tgbg_binarization ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_10.g");

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_10.g.out")
    );

}
*/

BOOST_AUTO_TEST_CASE( tgbg_binarization_with_star ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_11.g");

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_11.g.out")
    );

}


BOOST_AUTO_TEST_CASE( tgbg_binarization_with_attrs ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_12.g");

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_12.g.out")
    );

}


BOOST_AUTO_TEST_CASE( tgbg_binarization_with_attrs_2 ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_13.g");

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_13.g.out")
    );

}


BOOST_AUTO_TEST_SUITE_END()

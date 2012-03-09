#include "tests.hpp"

#include "agenda.hpp"
#include "agenda_parser.tpl"
#include "avinput_parser.tpl"
#include "number_master.hpp"
#include "psi_lattice_writer.hpp"
#include "test_helpers.hpp"
#include "tgbg_combinator.tpl"


#ifdef RUN_PARSER
#error "RUN_PARSER already defined"
#endif
#define RUN_PARSER(FILE_RULES, FILE_INPUT, FILE_OUTPUT) \
    Lattice lattice; \
    typedef tgbg_combinator< \
        int, \
        Lattice::Score, \
        number_master, \
        semantics_stub<int, number_master, double> \
    > Combinator; \
    Combinator combinator; \
    combinator.add_rules(ROOT_DIR FILE_RULES); \
    typedef chart< \
        av_matrix<int, int>, \
        Lattice::Score, \
        Combinator::variant_type, \
        Combinator::rule_type \
    > Chart; \
    Chart ch(lattice); \
    number_master& master = combinator.get_master(); \
    registrar<std::string>& symbol_reg = combinator.get_symbol_registrar(); \
    registrar<std::string>& attribute_reg = combinator.get_attribute_registrar(); \
    registrar<std::string>& extra_attribute_reg = combinator.get_extra_attribute_registrar(); \
    simple_converter<int> converter(symbol_reg, attribute_reg, extra_attribute_reg); \
    std::vector<Combinator::rule_holder> local_rules; \
    avinput_parser< \
        int, \
        Combinator::rule_type, \
        int, \
        number_master, \
        Chart, \
        simple_converter<int>, \
        Combinator::rule_holder \
    > av_parser(master, ch, converter, local_rules); \
    BOOST_CHECK(av_parser.parse(slurp_file(ROOT_DIR FILE_INPUT))); \
    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter()); \
    typedef fifo_agenda<Chart::edge_descriptor> Agenda; \
    Agenda agenda; \
    agenda_parser< \
        av_matrix<int, int>, \
        Lattice::Score, \
        Combinator::variant_type, \
        Combinator::rule_type, \
        Combinator, \
        Agenda \
    > tgbg_parser(ch, combinator, agenda); \
    tgbg_parser.run(); \
    std::ostringstream osstr; \
    writer->writeLattice(lattice, osstr); \
    BOOST_CHECK_EQUAL(osstr.str(), slurp_file(ROOT_DIR FILE_OUTPUT));


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


BOOST_AUTO_TEST_CASE( avinput ) {

    Lattice lattice;

    typedef tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > Combinator;

    typedef chart<
        av_matrix<int, int>,
        Lattice::Score,
        Combinator::variant_type,
        Combinator::rule_type
    > Chart;

    Chart ch(lattice);
    number_master master;
    registrar<std::string> symbol_reg;
    registrar<std::string> attribute_reg;
    registrar<std::string> extra_attribute_reg;
    simple_converter<int> converter(symbol_reg, attribute_reg, extra_attribute_reg);

    std::vector<Combinator::rule_holder> local_rules;

    avinput_parser<
        int,
        Combinator::rule_type,
        int,
        number_master,
        Chart,
        simple_converter<int>,
        Combinator::rule_holder
    > av_parser(
        master,
        ch,
        converter,
        local_rules
    );

    BOOST_CHECK(av_parser.parse(slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/av_1.i")));

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter());
    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/av_1.i.out")
    );

}


BOOST_AUTO_TEST_CASE( tgbg_parsing ) {
    RUN_PARSER(
        "tools/parsers/gobio/t/files/rules_3.g",
        "tools/parsers/gobio/t/files/av_1.i",
        "tools/parsers/gobio/t/files/rules_3.g.out"
    );
}

/* TEST PASSES BUT TOO SLOW (> 2 min)
BOOST_AUTO_TEST_CASE( tgbg_parsing_tougher ) {
    RUN_PARSER(
        "tools/parsers/gobio/t/files/rules_5.g",
        "tools/parsers/gobio/t/files/av_2.i",
        "tools/parsers/gobio/t/files/av_2.i.out"
    );
}
// */

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


BOOST_AUTO_TEST_CASE( tgbg_prehook ) {
    RUN_PARSER(
        "tools/parsers/gobio/t/files/rules_14.g",
        "tools/parsers/gobio/t/files/av_3.i",
        "tools/parsers/gobio/t/files/av_3.i.out"
    );
}


BOOST_AUTO_TEST_CASE( tgbg_finalhook ) {
    RUN_PARSER(
        "tools/parsers/gobio/t/files/rules_16.g",
        "tools/parsers/gobio/t/files/av_4.i",
        "tools/parsers/gobio/t/files/av_4.i.out"
    );
}


BOOST_AUTO_TEST_CASE( setscore_factor ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_23.g");
    tgbg.compile_all_rules();

    std::ostringstream osstr;
    tgbg.print_rules(osstr);
    BOOST_CHECK_EQUAL(
        osstr.str(),
        slurp_file(ROOT_DIR "tools/parsers/gobio/t/files/rules_23.g.out")
    );

}


BOOST_AUTO_TEST_CASE( options ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_28.g");

    BOOST_CHECK_EQUAL(tgbg.get_option("super"), "yes");
    BOOST_CHECK_EQUAL(tgbg.get_option("oho"), "no");
    BOOST_CHECK_EQUAL(tgbg.get_option("bla"), "bla");
    BOOST_CHECK_EQUAL(tgbg.get_option("unknown"), "");

}


BOOST_AUTO_TEST_CASE( limits ) {

    tgbg_combinator<
        int,
        Lattice::Score,
        number_master,
        semantics_stub<int, number_master, double>
    > tgbg;
    tgbg.add_rules(ROOT_DIR "tools/parsers/gobio/t/files/rules_28.g");

    BOOST_CHECK_EQUAL(tgbg.get_nb_limits(), 3);
    BOOST_CHECK_EQUAL(tgbg.get_limit_threshold(2), 1000000000);
    BOOST_CHECK_EQUAL(tgbg.get_limit_threshold(1), 200);
    BOOST_CHECK_EQUAL(tgbg.get_limit_threshold(0), 100);
    BOOST_CHECK_CLOSE(tgbg.get_limit_min_score(0), -0.5, 0.00001);
    BOOST_CHECK_CLOSE(tgbg.get_limit_min_score(1), 2.3, 0.00001);
    BOOST_CHECK_CLOSE(tgbg.get_limit_min_score(2), 100, 0.00001);

}


BOOST_AUTO_TEST_SUITE_END()

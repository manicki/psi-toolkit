#include "tests.hpp"

#include "agenda.hpp"
#include "chart.tpl"
#include "lattice_preparators.hpp"
#include "simple_cfg_combinator.tpl"
#include "test_helpers.hpp"


#define ADD_BINARY_RULE(A,B,C) (combinator.add_binary_rule((B),(C),simple_cfg_rule<char>((A))))
#define ADD_UNARY_RULE(A,B) (combinator.add_unary_rule((B),simple_cfg_rule<char>((A))))


BOOST_AUTO_TEST_SUITE( gobio_parser )


BOOST_AUTO_TEST_CASE( combinator ) {

    simple_cfg_combinator<char, simple_cfg_rule<char> > combinator;

    combinator.add_unary_rule('X', simple_cfg_rule<char>('S', 1.0));
    combinator.add_binary_rule('a', 'Y', simple_cfg_rule<char>('X', 1.0));
    combinator.add_binary_rule('c', 'Y', simple_cfg_rule<char>('X', 1.0));
    combinator.add_binary_rule('c', 'Y', simple_cfg_rule<char>('S', 1.0));
    combinator.add_binary_rule('X', 'b', simple_cfg_rule<char>('Y', 1.0));
    combinator.add_binary_rule('X', 'd', simple_cfg_rule<char>('Y', 1.0));
    combinator.add_unary_rule('b', simple_cfg_rule<char>('Y', 1.0));
    combinator.add_binary_rule('c', 'Y', simple_cfg_rule<char>('S', 2.0));
    combinator.add_unary_rule('b', simple_cfg_rule<char>('T', 1.0));

    BOOST_CHECK_EQUAL(combinator.combine_unary('b').size(), 2);
    BOOST_CHECK_EQUAL(combinator.combine_unary('d').size(), 0);
    BOOST_CHECK_EQUAL(combinator.combine_binary('a', 'Y').size(), 1);
    BOOST_CHECK_EQUAL(combinator.combine_binary('c', 'Y').size(), 3);
    BOOST_CHECK_EQUAL(combinator.combine_binary('b', 'Y').size(), 0);

}


BOOST_AUTO_TEST_CASE( helpers ) {

    Lattice lattice;
    lattice_preparators::prepareLatticeWithOneSymbolTokens(lattice, "baaaaa");

    fifo_agenda<chart<char, double, int, simple_cfg_rule<char> >::edge_descriptor> agenda;
    simple_cfg_combinator<char, simple_cfg_rule<char> > combinator;

    chart<char, double, int, simple_cfg_rule<char> > ch(lattice);

    BOOST_CHECK_EQUAL(count_vertices(ch), 7);
/*
    BOOST_CHECK_EQUAL(count_out_edges(ch), 6);
    BOOST_CHECK(is_consistent(ch));

    ADD_UNARY_RULE('B', 'b');
    ADD_UNARY_RULE('A', 'a');
    ADD_BINARY_RULE('X', 'B', 'A');
    ADD_BINARY_RULE('X', 'X', 'A');
    ADD_UNARY_RULE('S', 'X');

    agenda_parser<
    char, double, int,
    simple_cfg_rule<char> ,
    simple_cfg_combinator<char, simple_cfg_rule<char> >,
    fifo_agenda<chart<char, double, int, simple_cfg_rule<char> >::edge_descriptor> >
    parser(ch, combinator, agenda);

    parser.run();

    BOOST_CHECK(check_parse_results(ch,"SX"));
    BOOST_CHECK(is_consistent(ch));
*/
}


BOOST_AUTO_TEST_SUITE_END()

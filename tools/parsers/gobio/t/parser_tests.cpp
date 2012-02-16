#include "tests.hpp"

#include "agenda.hpp"
#include "agenda_parser.tpl"
#include "chart.tpl"
#include "lattice_preparators.hpp"
#include "simple_cfg_combinator.tpl"
#include "test_helpers.hpp"


#define ADD_BINARY_RULE(A, B, C) \
    (combinator.add_binary_rule((B), (C), simple_cfg_rule<std::string>((A))))
#define ADD_UNARY_RULE(A, B) \
    (combinator.add_unary_rule((B), simple_cfg_rule<std::string>((A))))


#ifdef INIT_PARSER_TEST
#error "INIT_PARSER_TEST already defined"
#endif
#define INIT_PARSER_TEST(input_string) \
    typedef Lattice::EdgeDescriptor Edge; \
    typedef std::string Category; \
    typedef Lattice::Score Score; \
    typedef std::string Variant; \
    typedef simple_cfg_rule<Category> Rule; \
    typedef simple_cfg_combinator<Category, Rule> Combinator; \
    typedef fifo_agenda<Edge> Agenda; \
    typedef chart<Category, Score, Variant, Rule, simple_marked_edges_index> Chart; \
    typedef agenda_parser< \
        Category, \
        Score, \
        Variant, \
        Rule, \
        Combinator, \
        Agenda, \
        simple_marked_edges_index \
    > Parser; \
    Lattice lattice; \
    lattice_preparators::prepareLatticeWithOneSymbolTokens(lattice, (input_string)); \
    Agenda agenda; \
    Combinator combinator; \
    Chart ch(lattice);


#ifdef RUN_PARSER_TEST
#error "RUN_PARSER TEST already defined"
#endif
#define RUN_PARSER_TEST \
    Parser parser(ch, combinator, agenda); \
    parser.run(); \
    BOOST_CHECK(is_consistent(ch));


bool check_parse_results(
    chart<
        std::string,
        Lattice::Score,
        std::string,
        simple_cfg_rule<std::string>,
        simple_marked_edges_index
    >& ch,
    std::set<std::string> scats
) {

    typedef chart<
        std::string,
        Lattice::Score,
        std::string,
        simple_cfg_rule<std::string>,
        simple_marked_edges_index
    > Chart;
    Chart::vertex_descriptor firstVertex = ch.getFirstVertex();
    Chart::out_edge_iterator pe = ch.out_edges(firstVertex);

    std::set<std::string> result_cats;
    while (pe.hasNext()) {
        Chart::edge_descriptor edge = pe.next();
        if (ch.edge_target(edge) == ch.getLastVertex()) {
            result_cats.insert(ch.edge_category(edge));
        }
    }

    if (result_cats.size() != scats.size()) {
        return false;
    }
    BOOST_FOREACH(std::string scat, scats) {
        if (result_cats.find(scat) == result_cats.end()) {
            return false;
        }
    }
    return true;

}


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

    typedef Lattice::EdgeDescriptor Edge;
    typedef std::string Category;
    typedef Lattice::Score Score;
    typedef std::string Variant;
    typedef simple_cfg_rule<Category> Rule;
    typedef simple_cfg_combinator<Category, Rule> Combinator;
    typedef fifo_agenda<Edge> Agenda;
    typedef chart<Category, Score, Variant, Rule, simple_marked_edges_index> Chart;
    typedef agenda_parser<
        Category,
        Score,
        Variant,
        Rule,
        Combinator,
        Agenda,
        simple_marked_edges_index
    > Parser;

    Lattice lattice;
    lattice_preparators::prepareLatticeWithOneSymbolTokens(lattice, "baaaaa");

    Agenda agenda;
    Combinator combinator;

    Chart ch(lattice);

    BOOST_CHECK_EQUAL(count_vertices(ch), 7);
    BOOST_CHECK_EQUAL(count_out_edges(ch), 6);
    BOOST_CHECK(is_consistent(ch));

    ADD_UNARY_RULE("B", "b");
    ADD_UNARY_RULE("A", "a");
    ADD_BINARY_RULE("X", "B", "A");
    ADD_BINARY_RULE("X", "X", "A");
    ADD_UNARY_RULE("S", "X");

    Parser parser(ch, combinator, agenda);

    parser.run();

    std::set<Category> expectedParseResults;
    expectedParseResults.insert("S");
    expectedParseResults.insert("X");
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
    BOOST_CHECK(is_consistent(ch));

}


BOOST_AUTO_TEST_CASE( parser_zero ) {
    INIT_PARSER_TEST("abc");
    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}

// /* generates edge self-reference error / infinite loop
BOOST_AUTO_TEST_CASE( parser_chain ) {
    INIT_PARSER_TEST("a");

    ADD_UNARY_RULE("A", "a");
    ADD_UNARY_RULE("B", "A");
    ADD_UNARY_RULE("B", "a");
    ADD_UNARY_RULE("X", "B");
    // ADD_UNARY_RULE("X", "X"); // generates edge self-reference error
    ADD_UNARY_RULE("S", "X");
    // ADD_UNARY_RULE("X", "S"); // generates an infinite loop

    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    expectedParseResults.insert("a");
    expectedParseResults.insert("A");
    expectedParseResults.insert("B");
    expectedParseResults.insert("X");
    expectedParseResults.insert("S");
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}
// */

BOOST_AUTO_TEST_CASE( parser_simple_binary ) {
    INIT_PARSER_TEST("ab");

    ADD_BINARY_RULE("S", "a", "b");

    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    expectedParseResults.insert("S");
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}


BOOST_AUTO_TEST_CASE( parser_simple_recursion ) {
    INIT_PARSER_TEST("baaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    ADD_UNARY_RULE("A", "a");
    ADD_UNARY_RULE("B", "b");
    ADD_BINARY_RULE("X", "B", "A");
    ADD_BINARY_RULE("X", "X", "A");
    ADD_UNARY_RULE("S", "X");

    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    expectedParseResults.insert("S");
    expectedParseResults.insert("X");
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}


BOOST_AUTO_TEST_CASE( parser_max ) {
    INIT_PARSER_TEST("aaaaaaaaaa");

    ADD_BINARY_RULE("S", "S", "S");
    ADD_UNARY_RULE("S", "a");

    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    expectedParseResults.insert("S");
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}


BOOST_AUTO_TEST_CASE( parser_anbncmdm ) {
    INIT_PARSER_TEST("aaaaaaaaaabbbbbbbbbbcccddd");

    ADD_UNARY_RULE("A","a");
    ADD_UNARY_RULE("B","b");
    ADD_UNARY_RULE("C","c");
    ADD_UNARY_RULE("D","d");

    ADD_BINARY_RULE("S", "X", "Y");

    ADD_BINARY_RULE("X", "A", "B");
    ADD_BINARY_RULE("I", "X", "B");
    ADD_BINARY_RULE("X", "A", "I");

    ADD_BINARY_RULE("Y", "C", "D");
    ADD_BINARY_RULE("J", "Y", "D");
    ADD_BINARY_RULE("Y", "C", "J");

    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    expectedParseResults.insert("S");
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}


BOOST_AUTO_TEST_CASE( parser_anbncmdm_fails ) {
    INIT_PARSER_TEST("aaaaaaaaaabbbbbbbbbbccccddd");

    ADD_UNARY_RULE("A", "a");
    ADD_UNARY_RULE("B", "b");
    ADD_UNARY_RULE("C", "c");
    ADD_UNARY_RULE("D", "d");

    ADD_BINARY_RULE("S", "X", "Y");

    ADD_BINARY_RULE("X", "A", "B");
    ADD_BINARY_RULE("I", "X", "B");
    ADD_BINARY_RULE("X", "A", "I");

    ADD_BINARY_RULE("Y", "C", "D");
    ADD_BINARY_RULE("J", "Y", "D");
    ADD_BINARY_RULE("Y", "C", "J");

    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}


BOOST_AUTO_TEST_CASE( parser_paraling ) {
    INIT_PARSER_TEST("aanvanrnv");

    ADD_UNARY_RULE("N", "n");
    ADD_BINARY_RULE("N", "n", "R");
    ADD_BINARY_RULE("N", "a", "N");
    ADD_BINARY_RULE("V", "v", "N");
    ADD_BINARY_RULE("S", "N", "V");
    ADD_BINARY_RULE("R", "r", "V");
    ADD_BINARY_RULE("R", "r", "W");
    ADD_BINARY_RULE("W", "N", "v");

    RUN_PARSER_TEST;
    std::set<Category> expectedParseResults;
    expectedParseResults.insert("S");
    BOOST_CHECK(check_parse_results(ch, expectedParseResults));
}


BOOST_AUTO_TEST_SUITE_END()


#undef INIT_PARSER_TEST
#undef RUN_PARSER_TEST
#undef ADD_BINARY_RULE
#undef ADD_UNARY_RULE

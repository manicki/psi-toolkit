#include "tests.hpp"

#include "chart.tpl"
#include "lattice.hpp"
#include "lattice_preparators.hpp"
#include "test_helpers.hpp"


class int_rule {
private:
    int rule_no_;

public:
    int_rule(int rule_no) : rule_no_(rule_no) { }

    int category() const { return rule_no_; }
    double score() const { return 0.0; }
    int rule_no() const { return rule_no_; }
};


BOOST_AUTO_TEST_SUITE( gobio_chart )


BOOST_AUTO_TEST_CASE( chart_vertices ) {

    Lattice lattice;
    typedef chart<std::string, double, int, int_rule> simple_chart;
    simple_chart ch(lattice);

    BOOST_CHECK_EQUAL(count_vertices(ch), 1);

    lattice.appendString(std::string(9, 'a'));

    BOOST_CHECK_EQUAL(count_vertices(ch), 10);

    lattice.appendStringWithSymbols(std::string(15, 'a'));

    BOOST_CHECK_EQUAL(count_vertices(ch), 25);

    lattice.appendStringWithSymbols(std::string(5000, 'a'));

    BOOST_CHECK_EQUAL(count_vertices(ch), 5025);

}


BOOST_AUTO_TEST_CASE( chart_vertices_with_preparator ) {

    Lattice lattice;
    typedef chart<std::string, double, int, int_rule> simple_chart;
    simple_chart ch(lattice);

    BOOST_CHECK_EQUAL(count_vertices(ch), 1);

    lattice_preparators::prepareLatticeWithOneSymbolTokens(lattice, "abccdddd");

    BOOST_CHECK_EQUAL(count_vertices(ch), 9);

}


BOOST_AUTO_TEST_CASE( chart_edges ) {

    Lattice lattice(std::string(7, 's'));
    typedef chart<std::string, double, int, int_rule> simple_chart;
    simple_chart ch(lattice);

    BOOST_CHECK_EQUAL(count_vertices(ch), 8);

    std::vector<simple_chart::vertex_descriptor> v(8);
    for (int i=0; i<8; ++i) {
        v[i] = lattice.getVertexForRawCharIndex(i);
    }

    BOOST_CHECK_EQUAL(count_vertices(ch), 8);

    BOOST_CHECK_EQUAL(count_out_edges(ch), 0);
    BOOST_CHECK_EQUAL(count_in_edges(ch), 0);

    ch.add_edge(v[1], v[2], "a", 2.0, 0);
    ch.add_edge(v[2], v[3], "b", -1.0, 515);
    ch.add_edge(v[0], v[1], "a", 3.0, 1);
    ch.add_edge(v[3], v[6], "e", 1.5, 0);
    ch.add_edge(v[6], v[7], "a", 1.1, 0);

    ch.add_edge(v[6], v[7], "b", 1.1, 0);
    ch.add_edge(v[6], v[7], "c", 1.1, 0);
    ch.add_edge(v[4], v[7], "c", 2.555, 23);
    ch.add_edge(v[3], v[4], "c", 2.555, 23);
    ch.add_edge(v[0], v[4], "c", 2.555, 23);

    ch.add_edge(v[6], v[7], "d", 1.3, 1);

    BOOST_CHECK_EQUAL(count_out_edges(ch), 11);
    BOOST_CHECK_EQUAL(count_in_edges(ch), 11);

    BOOST_CHECK_EQUAL(count_marked_out_edges(ch), 0);
    BOOST_CHECK_EQUAL(count_marked_in_edges(ch), 0);

    BOOST_CHECK_EQUAL(count_vertices(ch), 8);

    BOOST_CHECK(is_consistent(ch));

}


BOOST_AUTO_TEST_CASE( chart_mask ) {

    Lattice lattice;
    lattice_preparators::prepareLatticeWithOneSymbolTokens(lattice, "abccdddd");
    typedef chart<std::string, double, int, int_rule> simple_chart;
    simple_chart ch(lattice);

    BOOST_CHECK_EQUAL(count_out_edges(ch), 8);
    BOOST_CHECK_EQUAL(count_in_edges(ch), 8);

    ch.setTagMask(lattice.getLayerTagManager().anyTag());

    BOOST_CHECK_EQUAL(count_out_edges(ch), 16);
    BOOST_CHECK_EQUAL(count_in_edges(ch), 16);

    ch.setTagMask(lattice.getLayerTagManager().getMask(std::list<std::string>()));

    BOOST_CHECK_EQUAL(count_out_edges(ch), 0);
    BOOST_CHECK_EQUAL(count_in_edges(ch), 0);

}


BOOST_AUTO_TEST_CASE( chart_empty ) {

    Lattice lattice(std::string(2012, 's'));
    typedef chart<std::string, double, int, int_rule> simple_chart;
    simple_chart ch(lattice);

    BOOST_CHECK_EQUAL(count_out_edges(ch), 0);
    BOOST_CHECK_EQUAL(count_in_edges(ch), 0);

    BOOST_CHECK_EQUAL(count_marked_out_edges(ch), 0);
    BOOST_CHECK_EQUAL(count_marked_in_edges(ch), 0);

    BOOST_CHECK_EQUAL(count_vertices(ch), 2013);

    BOOST_CHECK(is_consistent(ch));

}


BOOST_AUTO_TEST_CASE( chart_linear ) {

    const int nb_vertices = 200;

    Lattice lattice(std::string(nb_vertices-1, 's'));
    typedef chart<std::string, double, int, int_rule> sample_chart;
    sample_chart ch(lattice);

    for (int i=0; i<nb_vertices; ++i) {
        if (i > 0) {
            for (char c='A'; c<='Z'; ++c) {
                ch.add_edge(
                    lattice.getVertexForRawCharIndex(i-1),
                    lattice.getVertexForRawCharIndex(i),
                    std::string(1, c),
                    1.4,
                    1
                );
            }
        }
    }

    BOOST_CHECK_EQUAL(count_out_edges(ch), 26*(nb_vertices-1));
    BOOST_CHECK_EQUAL(count_in_edges(ch), 26*(nb_vertices-1));

    BOOST_CHECK_EQUAL(count_marked_out_edges(ch), 0);
    BOOST_CHECK_EQUAL(count_marked_in_edges(ch), 0);

    BOOST_CHECK_EQUAL(count_vertices(ch), nb_vertices);

    BOOST_CHECK(is_consistent(ch));

}


BOOST_AUTO_TEST_CASE( chart_complete ) {

    const int nb_vertices = 30;

    Lattice lattice(std::string(nb_vertices-1, 's'));
    typedef chart<std::string, double, int, int_rule> simple_chart;
    simple_chart ch(lattice);

    for (int i=0; i<nb_vertices; ++i) {
        for (int j=0; j<i; ++j) {
            for (char c='A'; c<='Z'; ++c) {
                ch.add_edge(
                    lattice.getVertexForRawCharIndex(j),
                    lattice.getVertexForRawCharIndex(i),
                    std::string(1, c),
                    1.3,
                    1
                );
            }
        }
    }

    BOOST_CHECK_EQUAL(count_out_edges(ch), 26*(nb_vertices*(nb_vertices-1))/2);
    BOOST_CHECK_EQUAL(count_in_edges(ch), 26*(nb_vertices*(nb_vertices-1))/2);

    BOOST_CHECK_EQUAL(count_marked_out_edges(ch), 0);
    BOOST_CHECK_EQUAL(count_marked_in_edges(ch), 0);

    BOOST_CHECK_EQUAL(count_vertices(ch), nb_vertices);

    BOOST_CHECK(is_consistent(ch));

}


BOOST_AUTO_TEST_CASE( chart_marked ) {

    const int nb_vertices = 60;

    Lattice lattice(std::string(nb_vertices-1, 's'));
    typedef chart<std::string, double, int, int_rule> simple_chart;
    simple_chart ch(lattice);

    std::vector<simple_chart::edge_descriptor> e;

    for (int i=0; i<nb_vertices; ++i) {
        if (i > 0) {

            e.push_back(ch.add_edge(
                lattice.getVertexForRawCharIndex(i-1),
                lattice.getVertexForRawCharIndex(i),
                "a",
                1.4,
                1
            ).first);

            ch.mark_edge(ch.add_edge(
                lattice.getVertexForRawCharIndex(i-1),
                lattice.getVertexForRawCharIndex(i),
                "b",
                1.4,
                1
            ).first);

            ch.add_edge(
                lattice.getVertexForRawCharIndex(i-1),
                lattice.getVertexForRawCharIndex(i),
                "e",
                1.4,
                1
            );

        }
    }

    for (int i=nb_vertices/3; i<(nb_vertices/3)*2; ++i) {
        for (int j=nb_vertices/3; j<i; ++j) {

            ch.add_edge(
                lattice.getVertexForRawCharIndex(j),
                lattice.getVertexForRawCharIndex(i),
                "c",
                1.4,
                1
            );

            ch.mark_edge(ch.add_edge(
                lattice.getVertexForRawCharIndex(j),
                lattice.getVertexForRawCharIndex(i),
                "d",
                1.4,
                1
            ).first);

        }
    }

    for (int i=0; i<nb_vertices-1; ++i) {
        ch.mark_edge(e[i]);
    }

    int expected_nb_edges = (nb_vertices-1)*3 + (nb_vertices/3)*(nb_vertices/3-1);

    BOOST_CHECK_EQUAL(count_out_edges(ch), expected_nb_edges);
    BOOST_CHECK_EQUAL(count_in_edges(ch), expected_nb_edges);

    int expected_nb_out_edges = (nb_vertices-1)*2 + ((nb_vertices/3)*(nb_vertices/3-1))/2;

    BOOST_CHECK_EQUAL(count_marked_out_edges(ch), expected_nb_out_edges);
    BOOST_CHECK_EQUAL(count_marked_in_edges(ch), expected_nb_out_edges);

    BOOST_CHECK_EQUAL(count_vertices(ch), nb_vertices);

    BOOST_CHECK(is_consistent(ch));

}


BOOST_AUTO_TEST_SUITE_END()

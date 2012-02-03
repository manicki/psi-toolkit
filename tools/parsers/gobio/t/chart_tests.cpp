#include "tests.hpp"

#include "chart.tpl"
#include "lattice.hpp"
#include "lattice_preparators.hpp"
#include "test_helpers.hpp"


BOOST_AUTO_TEST_SUITE( gobio_chart )


BOOST_AUTO_TEST_CASE( vertices ) {

    Lattice lattice;
    typedef chart<char, double, int, int> simple_chart;
    simple_chart ch(lattice);

    BOOST_CHECK_EQUAL(count_vertices(ch), 0);

    lattice_preparators::prepareLatticeWithOneSymbolTokens(lattice, "abccdddd");

    BOOST_CHECK_EQUAL(count_vertices(ch), 9);

}


BOOST_AUTO_TEST_SUITE_END()

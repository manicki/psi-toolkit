#include "tests.hpp"

#include "NDFSA.hpp"
#include "Algorithms.hpp"

BOOST_AUTO_TEST_SUITE( psi_query )

BOOST_AUTO_TEST_CASE(epsremove_test)
{
    // create automaton for (a+b+)?

    psi::NDFSA<> a(psi::Symbol('a'));
    psi::NDFSA<> b(psi::Symbol('b'));

    psi::kleene_plus(a);
    psi::kleene_plus(b);
    psi::concatenate(a, b);
    psi::kleene_option(a);

    // remove epsilon transitions

    a.print();
    std::cout << std::endl;

    psi::epsRemove(a);

    a.print();

    // check for strings

    std::string test1 = "aabb";
    std::string test2 = "aaaabbbbbb";
    std::string test3 = "aaaa";
    std::string test4 = "bbbb";

    BOOST_CHECK_EQUAL(a.in(test1.begin(), test1.end()), 1);
    BOOST_CHECK_EQUAL(a.in(test2.begin(), test2.end()), 1);
    BOOST_CHECK_EQUAL(a.in(test3.begin(), test3.end()), 0);
    BOOST_CHECK_EQUAL(a.in(test4.begin(), test4.end()), 0);
}

BOOST_AUTO_TEST_SUITE_END()

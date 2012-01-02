#include "tests.hpp"

#include "DFSA.hpp"
#include "Algorithms.hpp"

BOOST_AUTO_TEST_SUITE( psi_query )

BOOST_AUTO_TEST_CASE(DFSA_test)
{
	psi::DFSA<> fsa;
    psi::State q0 = fsa.addState();
    psi::State q1 = fsa.addState();
    psi::State q2 = fsa.addState();
    psi::State q3 = fsa.addState();

    fsa.addArc(q0, psi::Arc<>('a', q1));
    fsa.addArc(q0, psi::Arc<>('b', q1));
    fsa.addArc(q0, psi::Arc<>('c', q1));

    fsa.addArc(q1, psi::Arc<>('a', q1));
    fsa.addArc(q1, psi::Arc<>('b', q2));
    fsa.addArc(q1, psi::Arc<>('c', q3));

    fsa.addArc(q2, psi::Arc<>('a', q1));
    fsa.addArc(q2, psi::Arc<>('b', q3));
    fsa.addArc(q2, psi::Arc<>('c', q3));

    fsa.addArc(q3, psi::Arc<>('a', q1));
    fsa.addArc(q3, psi::Arc<>('b', q3));
    fsa.addArc(q3, psi::Arc<>('c', q3));


    fsa.setStartState(q0);
    fsa.setEndState(q3);

    std::string test1 = "abc";
    std::string test2 = "cba";
    std::string test3 = "abb";
    std::string test4 = "acbaabaac";

    BOOST_CHECK_EQUAL(fsa.in(test1.begin(), test1.end()) , 1);
    BOOST_CHECK_EQUAL(fsa.in(test2.begin(), test2.end()) , 0);
    BOOST_CHECK_EQUAL(fsa.in(test3.begin(),test3.end()) , 1);
    BOOST_CHECK_EQUAL(fsa.in(test4.begin(),test4.end()) , 1);
    psi::reverse(fsa);

    BOOST_CHECK_EQUAL(fsa.in(test1.begin(), test1.end()) , 0);
    BOOST_CHECK_EQUAL(fsa.in(test2.begin(), test2.end()) , 0);

    //State test///////////////////////////////////////
    BOOST_CHECK_EQUAL(fsa.isStartState(q0) , 1);
    BOOST_CHECK_EQUAL(fsa.isEndState(q0) , 0);
    BOOST_CHECK_EQUAL(fsa.isEndState(q3) , 1);
    ///////////////////////////////////////////////////
}

BOOST_AUTO_TEST_SUITE_END()

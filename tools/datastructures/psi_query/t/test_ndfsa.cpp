#include "tests/tests.hpp"
#include "NDFSA.hpp"
#include "Algorithms.hpp"

struct Tescik {
    psi::NDFSA<> ndfsa;

    int poprawneSize;
    std::string* poprawne;
    int niePoprawneSize;
    std::string* niePoprawne;

    Tescik(int poprawneSize_ = 0, int niePoprawneSize_ = 0)
      : poprawneSize(poprawneSize_), niePoprawneSize(niePoprawneSize_) {
        poprawne = new std::string[poprawneSize + 1];
        niePoprawne = new std::string[niePoprawneSize + 1];
    }

    ~Tescik() {
        delete[] poprawne;
        delete[] niePoprawne;
    }

};

BOOST_AUTO_TEST_CASE(NDFSA_test)
{
    //Automat powinien akceptowac język w postaci:
    //{a + b}* b {a + b} b {a + b}*
    Tescik a(8,7);
    psi::State q0 = a.ndfsa.addState();
    psi::State q1 = a.ndfsa.addState();
    psi::State q2 = a.ndfsa.addState();
    psi::State q3 = a.ndfsa.addState();

    a.ndfsa.addArc(q0, psi::Arc<>('a', q0));
    a.ndfsa.addArc(q0, psi::Arc<>('b', q0));
    a.ndfsa.addArc(q0, psi::Arc<>('b', q1));

    a.ndfsa.addArc(q1, psi::Arc<>('a', q2));
    a.ndfsa.addArc(q1, psi::Arc<>('b', q2));

    a.ndfsa.addArc(q2, psi::Arc<>('b', q3));

    a.ndfsa.addArc(q3, psi::Arc<>('a', q3));
    a.ndfsa.addArc(q3, psi::Arc<>('b', q3));


    a.ndfsa.setStartState(q0);
    a.ndfsa.setEndState(q3);

    //Te powinny być zaakceptowane
    a.poprawne[0] = "aaabbbabababaabab";
    a.poprawne[1] = "aaaaabbbbbbababbabaabab";
    a.poprawne[2] = "ababa";
    a.poprawne[3] = "abababbba";
    a.poprawne[4] = "bbbaabbbbaa";
    a.poprawne[5] = "bbbbb";
    a.poprawne[6] = "bbb";
    a.poprawne[7] = "babbbb";

    //Tych nie powinien zaakceptować
    a.niePoprawne[0] = "aaaaaaaaaaaab";
    a.niePoprawne[1] = "aabbaaabbaaabb";
    a.niePoprawne[2] = "aaaaa";
    a.niePoprawne[3] = "ababaaaaaa";
    a.niePoprawne[4] = "bb";
    a.niePoprawne[5] = "bbaaaaaa";
    a.niePoprawne[6] = "bab";

    for (int i = 0;i < a.poprawneSize; ++i) {
        BOOST_CHECK_EQUAL(a.ndfsa.in(a.poprawne[i].begin(),
                                    a.poprawne[i].end()), true);
    }

    //for(int i = 0;i < a.niePoprawneSize; ++i) {
        //BOOST_CHECK_EQUAL(a.ndfsa.in(a.niePoprawne[i].begin(),
        //                            a.niePoprawne[i].end()), false);
    //}


    //Automat powinien akceptowac język w postaci:
    //{a + b}* c
    Tescik b(8,7);
    psi::State g0 = b.ndfsa.addState();
    psi::State g1 = b.ndfsa.addState();

    b.ndfsa.addArc(g0, psi::Arc<>('a', g0));
    b.ndfsa.addArc(g0, psi::Arc<>('b', g0));
    b.ndfsa.addArc(g0, psi::Arc<>('c', g1));

    b.ndfsa.setStartState(g0);
    b.ndfsa.setEndState(g1);

    //Te powinny być zaakceptowane
    b.poprawne[0] = "abababaabbbaaac";
    b.poprawne[1] = "aaaaabbbbbbababbabaababc";
    b.poprawne[2] = "c";
    b.poprawne[3] = "abc";
    b.poprawne[4] = "bbaac";
    b.poprawne[5] = "aaabbbc";
    b.poprawne[6] = "ac";
    b.poprawne[7] = "bc";

    //Tych nie powinien zaakceptować
    b.niePoprawne[0] = "abbbbaabab";
    b.niePoprawne[1] = "aabbaaabbaaabb";
    b.niePoprawne[2] = "aaaaa";
    b.niePoprawne[3] = "abbbbba";
    b.niePoprawne[4] = "bbcaaa";
    b.niePoprawne[5] = "abbbbaaacaaba";
    b.niePoprawne[6] = "bab";

    for (int i = 0;i < b.poprawneSize; ++i) {
        BOOST_CHECK_EQUAL(b.ndfsa.in(b.poprawne[i].begin(),
                                    b.poprawne[i].end()), true);
    }

    //for(int i = 0;i < b.niePoprawneSize; ++i) {
        //BOOST_CHECK_EQUAL(b.ndfsa.in(b.niePoprawne[i].begin(),
        //                            b.niePoprawne[i].end()), false);
    //}

    //Automat powinien akceptowac język w postaci:
    //{a + b + c}*
    Tescik c(12,7);
    psi::State d0 = c.ndfsa.addState();

    c.ndfsa.addArc(d0, psi::Arc<>('a', d0));
    c.ndfsa.addArc(d0, psi::Arc<>('b', d0));
    c.ndfsa.addArc(d0, psi::Arc<>('c', d0));

    c.ndfsa.setStartState(d0);
    c.ndfsa.setEndState(d0);

    //Te powinny być zaakceptowane
    c.poprawne[0] = "aaabcbbabababcaabab";
    c.poprawne[1] = "cccbbacbabab";
    c.poprawne[2] = "abcabca";
    c.poprawne[3] = "abababbba";
    c.poprawne[4] = "bcbacabbbbccccaa";
    c.poprawne[5] = "bbbcbb";
    c.poprawne[6] = "bcbbccccc";
    c.poprawne[7] = "bcabcbbb";
    c.poprawne[8] = "";
    c.poprawne[9] = "a";
    c.poprawne[10] = "b";
    c.poprawne[11] = "c";

    //Tych nie powinien zaakceptować
    c.niePoprawne[0] = "aaaaaaaaaaaab";
    c.niePoprawne[1] = "aabbaaabbaaabb";
    c.niePoprawne[2] = "aaaaa";
    c.niePoprawne[3] = "ababaaaaaa";
    c.niePoprawne[4] = "bb";
    c.niePoprawne[5] = "bbaaaaaa";
    c.niePoprawne[6] = "bab";

    for (int i = 0;i < c.poprawneSize; ++i) {
        BOOST_CHECK_EQUAL(c.ndfsa.in(c.poprawne[i].begin(),
                                    c.poprawne[i].end()), true);
    }

    //for(int i = 0;i < c.niePoprawneSize; ++i) {
        //BOOST_CHECK_EQUAL(c.ndfsa.in(c.niePoprawne[i].begin(),
        //                            c.niePoprawne[i].end()), false);
    //}


    //Automat powinien akceptowac język w postaci:
    //... MJD: wedlug mnie to b*ab*a(bb*ab*a)*
    Tescik d(3,7);
    psi::State e0 = d.ndfsa.addState();
    psi::State e1 = d.ndfsa.addState();
    psi::State e2 = d.ndfsa.addState();

    d.ndfsa.addArc(e0, psi::Arc<>('a', e1));
    d.ndfsa.addArc(e0, psi::Arc<>('b', e0));

    d.ndfsa.addArc(e1, psi::Arc<>('b', e1));
    d.ndfsa.addArc(e1, psi::Arc<>('a', e2));

    d.ndfsa.addArc(e2, psi::Arc<>('b', e0));

    d.ndfsa.setStartState(e0);
    d.ndfsa.setEndState(e2);

    //Te powinny być zaakceptowane
    d.poprawne[0] = "bbbbabbbababa";
    d.poprawne[1] = "aa";
    d.poprawne[2] = "bbbbbbbabbbbbabaa";

    //Tych nie powinien zaakceptować
    d.niePoprawne[0] = "aaaaaaaaaaaab";
    d.niePoprawne[1] = "abbbbbbbbb";
    d.niePoprawne[2] = "aabbbbbbbbb";
    d.niePoprawne[3] = "aabaabaab";
    d.niePoprawne[4] = "bbbbbbbbb";
    d.niePoprawne[5] = "a";
    d.niePoprawne[6] = "b";

    for (int i = 0;i < d.poprawneSize; ++i) {
        BOOST_CHECK_EQUAL(d.ndfsa.in(d.poprawne[i].begin(),
                                    d.poprawne[i].end()), true);
    }

    //for(int i = 0;i < d.niePoprawneSize; ++i) {
    //    BOOST_CHECK_EQUAL(d.ndfsa.in(d.niePoprawne[i].begin(),
    //                                d.niePoprawne[i].end()), false);
    //}
}



#ifndef ALGORITHMS_HDR
#define ALGORITHMS_HDR

#include <map>
#include <vector>
#include <set>
#include <boost/foreach.hpp>

#include "FSATypes.hpp"

/*******************************************************************************
# Algorithms

Algorithms for manipulation of all finite state automaton types included in the
library. Processing takes is "in-place", the first argument is destroyed and
replaced by the resulting automaton. Therefore applying a function to a finite
state automaton invalidates all previously used iterators and member objects.

## Synopsis

Creating an automaton that accepts the following regular expression: `(a|b|c)+aabb(cc)?`

    NDFSA<> fsa;

    NDFSA<> tfsa1("a");
    NDFSA<> tfsa2("b");
    NDFSA<> tfsa3("c");

    unify(fsa, tfsa1);
    unify(fsa, tfsa2);
    unify(fsa, tfsa3);

    kleene_plus(fsa);

    NDFSA<> tfsa4("aabb");

    concatenate(fsa, tfsa4);

    NDFSA<> tfsa5("cc");
    kleene_option(tfsa5);

    concatenate(fsa, tfsa5);

    minimize(fsa);

    std::cout << fsa.in("aabcccaaaabbcc") << std::endl; // prints 1
    std::cout << fsa.in("aabcccaaaabb") << std::endl; // prints 1
    std::cout << fsa.in("aabcccaaaabcc") << std::endl; // prints 0


*******************************************************************************/

namespace psi {

    template <typename Wrapper, typename FSA>
    void traverse(Wrapper &wrapper, FSA &fsa) {
        typedef typename Wrapper::state_type WrapperState;
        typedef typename Wrapper::arc_type WrapperArc;

        typedef typename FSA::state_type State;
        typedef typename FSA::arc_type Arc;

        std::map<WrapperState, State> mapper;
        std::vector<WrapperState> queue;

        BOOST_FOREACH( WrapperState s, wrapper.startFn() ) {
            State n = fsa.addState(true);
            mapper[s] = n;
            queue.push_back(s);
        }

        while (!queue.empty()) {
            WrapperState s = queue.back();
            queue.pop_back();

            if (!mapper.count(s)) {
                State n = fsa.addState();
                mapper[s] = n;
            }

            if (wrapper.finalFn(s))
                fsa.setEndState(mapper[s]);

            BOOST_FOREACH( WrapperArc arc, wrapper.arcFn(s) ) {
                if (!mapper.count(arc.getDest())) {
                    State n = fsa.addState();
                    mapper[arc.getDest()] = n;
                    queue.push_back(arc.getDest());
                }
                fsa.addArc(
                    mapper[s], Arc(arc.getSymbol(), mapper[arc.getDest()])
                );
            }
        }
    }

    template <typename FSA>
    class Reverser {
      public:
        typedef typename FSA::state_type state_type;
        typedef typename FSA::arc_type arc_type;
        typedef typename FSA::arc_iterator_type arc_iterator_type;

        Reverser(const FSA &fsa) : m_fsa(fsa) {
            for (state_type p = 0; (size_t)p < m_fsa.size(); p++) {
                ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(p);
                for (arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++) {
                    state_type temp = ait->getDest();
                    arc_type arc(*ait);
                    arc.setDest(p);
                    //std::cerr << "r: " << temp << std::endl;
                    reversedArcs[temp].insert(arc);
                }
            }
        }

        std::set<state_type> startFn() const {
            return m_fsa.getEndStates();
        }

        bool finalFn(state_type p) const {
            return m_fsa.isStartState(p);
        }

        std::set<arc_type, ArcSorter> arcFn(state_type p) const {
            //std::cerr << "revarcs: " << reversedArcs.size() << " " << p << std::endl;
            typename std::map<state_type, std::set<arc_type, ArcSorter> >
                ::const_iterator cit = reversedArcs.find(p);
            if (cit != reversedArcs.end())
                return cit->second;
            else
                return std::set<arc_type, ArcSorter>();
        }

      private:
        const FSA &m_fsa;
        std::map<state_type, std::set<arc_type, ArcSorter> > reversedArcs;
    };

    template <typename FSA>
    class Copier {
      public:
        typedef typename FSA::state_type state_type;
        typedef typename FSA::arc_type arc_type;
        typedef typename FSA::arc_iterator_type arc_iterator_type;

        Copier(const FSA &fsa) : m_fsa(fsa) {}

        std::set<state_type> startFn() const {
            return m_fsa.getStartStates();
        }

        bool finalFn(state_type p) const {
            return m_fsa.isEndState(p);
        }

        std::set<arc_type, ArcSorter> arcFn(state_type p) const {
            std::set<arc_type, ArcSorter> arcs;
            ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(p);
            for (arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++)
                arcs.insert(*ait);
            return arcs;
        }

      private:
        const FSA &m_fsa;
    };

    template <typename FSA1, typename FSA2>
    class Intersector {
      public:
        typedef std::pair<typename FSA1::state_type, typename FSA2::state_type> state_type;
        typedef Arc<typename FSA1::arc_type::symbol_type, state_type> arc_type;

        Intersector(const FSA1 &fsa1, const FSA2 &fsa2) : m_fsa1(fsa1), m_fsa2(fsa2) {}

        std::set<state_type> startFn() const {
            std::set<state_type> starts;
            BOOST_FOREACH( typename FSA1::state_type p1, m_fsa1.getStartStates() )
                BOOST_FOREACH( typename FSA2::state_type p2, m_fsa1.getStartStates() )
                    starts.insert(state_type(p1, p2));

            return starts;
        }

        bool finalFn(state_type p) const {
            return (m_fsa1.isEndState(p.first) && m_fsa2.isEndState(p.second));
        }

        std::set<arc_type, ArcSorter> arcFn(state_type p) const {
            std::set<arc_type, ArcSorter> arcs;

            ArcRange<typename FSA1::arc_iterator_type> arcRange1 = m_fsa1.getArcs(p.first);
            ArcRange<typename FSA2::arc_iterator_type> arcRange2 = m_fsa2.getArcs(p.second);

            for (typename FSA1::arc_iterator_type ait1 = arcRange1.first;
                ait1 != arcRange1.second;
                ait1++)
                for (typename FSA2::arc_iterator_type ait2 = arcRange2.first;
                    ait2 != arcRange2.second;
                    ait2++)
                    if (ait1->getSymbol() == ait2->getSymbol())
                       arcs.insert(
                           arc_type(
                               ait1->getSymbol(),
                               state_type(ait1->getDest(), ait2->getDest())
                           )
                       );

            return arcs;
        }

      private:
        const FSA1 &m_fsa1;
        const FSA2 &m_fsa2;
    };

    template <typename FSA>
    class EpsRemover {
      public:
        typedef typename FSA::state_type state_type;
        typedef typename FSA::arc_type arc_type;
        typedef typename FSA::arc_iterator_type arc_iterator_type;

        EpsRemover(const FSA &fsa) : m_fsa(fsa) {}

        std::set<state_type> startFn() const {
            return m_fsa.getStartStates();
        }

        bool finalFn(state_type p) const {
            BOOST_FOREACH(state_type q, m_fsa.epsClosure(p))
                if (m_fsa.isEndState(q))
                    return true;
            return false;
        }

        std::set<arc_type, ArcSorter> arcFn(State p) const {
            std::set<arc_type, ArcSorter> arcs;
            BOOST_FOREACH(state_type q, m_fsa.epsClosure(p)) {
                ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(q);
                for (arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++)
                    if (ait->getSymbol() != EPS)
                        arcs.insert(*ait);
            }
            return arcs;
        }

      private:
        const FSA &m_fsa;
    };

    template <typename FSA>
    class Determinizer {
      public:
        typedef std::set<typename FSA::state_type> state_type;
        typedef Arc<typename FSA::arc_type::symbol_type, state_type> arc_type;

        Determinizer(const FSA &fsa) : m_fsa(fsa) {}

        std::set<state_type> startFn() const {
            std::set<state_type> start;
            start.insert(m_fsa.getStartStates());
            return start;
        }

        bool finalFn(state_type p) const {
            BOOST_FOREACH(typename FSA::state_type q, p)
                if (m_fsa.isEndState(q))
                    return true;
            return false;
        }

        std::set<arc_type, ArcSorter> arcFn(state_type p) const {
            std::set<arc_type, ArcSorter> arcs;
            std::map<typename FSA::arc_type::symbol_type, arc_type> arcsMap;
            BOOST_FOREACH(typename FSA::state_type q, p) {
                ArcRange<typename FSA::arc_iterator_type> arcRange = m_fsa.getArcs(q);
                for (typename FSA::arc_iterator_type ait = arcRange.first;
                    ait != arcRange.second; ait++) {

                    arc_type& arc = arcsMap[ait->getSymbol()];
                    arc.setSymbol(ait->getSymbol());
                    const_cast<state_type&>(arc.getDest()).insert(ait->getDest());
                }
            }

            typedef typename std::map<typename FSA::arc_type::symbol_type, arc_type>
                ::value_type map_type;
            BOOST_FOREACH(map_type m, arcsMap)
                arcs.insert(m.second);

            return arcs;
        }

      private:
        const FSA &m_fsa;
    };

/*******************************************************************************
## Reverse

    template <typename FSA>
    void reverse(FSA &fsa);

Reverses the given automaton.

*******************************************************************************/
    template <typename FSA>
    void reverse(FSA &fsa) {
        FSA temp;
        Reverser<FSA> rvr(fsa);
        traverse(rvr, temp);
        fsa.swap(temp);
    }

/*******************************************************************************
## Reachable

    template <typename FSA>
    void reachable(FSA &fsa)

Keeps only reachable states.

*******************************************************************************/
    template <typename FSA>
    void reachable(FSA &fsa) {
        FSA temp;
        traverse(Copier<FSA>(fsa), temp);
        fsa.swap(temp);
    }

/*******************************************************************************
## Prune

    template <typename FSA>
    void prune(FSA &fsa)

*******************************************************************************/
    template <typename FSA>
    void prune(FSA &fsa) {
        reverse(fsa);
        reachable(fsa);
        reverse(fsa);
    }

/*******************************************************************************
## epsRemove

    template <typename FSA>
    void epsRemove(FSA &fsa)

Removes epsilon transitions from a non-deterministic automaton.

*******************************************************************************/
    template <typename FSA>
    void epsRemove(FSA &fsa) {
        FSA temp;
        EpsRemover<FSA> epsrm(fsa);
        traverse(epsrm, temp);
        fsa.swap(temp);
    }

/*******************************************************************************
## Determinize

Determinizes a non-deterministic automaton. Requires removal of epsilon
transitions first if present.

*******************************************************************************/
    template <typename FSA>
    void determinize(FSA &fsa) {
        FSA temp;
        Determinizer<FSA> dtr(fsa);
        traverse(dtr, temp);
        fsa.swap(temp);
    }

/*******************************************************************************
## Minimize

    template <typename FSA>
    void minimize(FSA &fsa)

Minimizes a non-deterministic automaton. Epsilon transtion removal and
determinization are automatically performed.

*******************************************************************************/
    template <typename FSA>
    void minimize(FSA &fsa) {
        epsRemove(fsa);
        reverse(fsa);
        determinize(fsa);
        reverse(fsa);
        determinize(fsa);
    }

/*******************************************************************************
## Unify

    template <typename FSA1, typename FSA2>
    void unify(FSA1 &dst, FSA2 &src)

Creates the union of two finite state automata. The result is saved in the first
argument. This results in an automation with epsilon transtions.

*******************************************************************************/
    template <typename FSA1, typename FSA2>
    void unify(FSA1 &dst, FSA2 &src) {
        Copier<FSA2> copier(src);
        traverse(copier, dst);
    }

/*******************************************************************************
## Intersect

    template <typename FSA1, typename FSA2>
    void intersect(FSA1 &dst, FSA2 &src)

Creates the intersection of two finite state automata. The result is saved in
the first argument.

*******************************************************************************/
    template <typename FSA1, typename FSA2>
    void intersect(FSA1 &dst, FSA2 &src) {
        FSA1 temp;
        Intersector<FSA1, FSA2> intersector(dst, src);
        traverse(intersector, temp);
        dst.swap(temp);
    }

/*******************************************************************************
## Concatenate

    template <typename FSA1, typename FSA2>
    void concatenate(FSA1 &dst, FSA2 &src)

Creates the concatenation of two finite state automata. The result is saved in
the first argument. This results in an automaton with epsilon transtions.

*******************************************************************************/
    template <typename FSA1, typename FSA2>
    void concatenate(FSA1 &dst, FSA2 &src) {
        std::set<typename FSA1::state_type> first_start  = dst.getStartStates();
        std::set<typename FSA1::state_type> first_end    = dst.getEndStates();

        Copier<FSA2> copier(src);
        traverse(copier, dst);

        std::set<typename FSA1::state_type> second_start = dst.getStartStates();

        BOOST_FOREACH(typename FSA1::state_type p, first_end) {
            BOOST_FOREACH(typename FSA1::state_type q, second_start) {
                if (!first_start.count(q)) {
                    dst.addArc(p, typename FSA1::arc_type(EPS, q));
                    dst.unsetStartState(q);
                }
            }
            dst.unsetEndState(p);
        }
    }

/*******************************************************************************
## Kleene Option

    template <typename FSA>
    void kleene_option(FSA &fsa)

Creates an optional finite state automaton. Tt accepts the language of the
argument automaton and the empty word. This results in an automaton with
epsilon transtions.

*******************************************************************************/
    template <typename FSA>
    void kleene_option(FSA &fsa) {
        const std::set<typename FSA::state_type> start  = fsa.getStartStates();
        const std::set<typename FSA::state_type> end    = fsa.getEndStates();

        typename FSA::state_type ns = fsa.addState();
        fsa.setStartState(ns);

        typename FSA::state_type ne = fsa.addState();
        fsa.setEndState(ne);

        BOOST_FOREACH(typename FSA::state_type p, start) {
            fsa.unsetStartState(p);
            fsa.addArc(ns, typename FSA::arc_type(EPS, p));
        }

        BOOST_FOREACH(typename FSA::state_type p, end) {
            fsa.unsetEndState(p);
            fsa.addArc(p, typename FSA::arc_type(EPS, ne));
        }
    }

/*******************************************************************************
## Kleene plus

    template <typename FSA>
    void kleene_plus(FSA &fsa)

Creates the non-empty Kleene closure of the passed automaton. The result will
accept the language of the input automaton repeated one to infinite times. This
results in an automaton with epsilon transtions.

*******************************************************************************/
    template <typename FSA>
    void kleene_plus(FSA &fsa) {
        const std::set<typename FSA::state_type> &start  = fsa.getStartStates();
        const std::set<typename FSA::state_type> &end    = fsa.getEndStates();

        BOOST_FOREACH(typename FSA::state_type p, end)
            BOOST_FOREACH(typename FSA::state_type q, start)
                fsa.addArc(p, typename FSA::arc_type(EPS, q));
    }

/*******************************************************************************
## Kleene star

    template <typename FSA>
    void kleene_star(FSA &fsa)

Similar as above, but accepts also the empty string. This results in an
automaton with epsilon transtions.

*******************************************************************************/
    template <typename FSA>
    void kleene_star(FSA &fsa) {
        kleene_option(fsa);
        kleene_plus(fsa);
    }
}

#endif

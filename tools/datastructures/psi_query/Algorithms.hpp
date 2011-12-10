#ifndef ALGORITHMS_HPP__
#define ALGORITHMS_HPP__

#include <map>
#include <vector>
#include <boost/foreach.hpp>

namespace psi {
 
    template <typename FSA>
    class Reverser {
      public:
        typedef typename FSA::state_type state_type;
        typedef typename FSA::arc_type arc_type;
        typedef typename FSA::arc_iterator_type arc_iterator_type;
        
        Reverser(const FSA &fsa) : m_fsa(fsa) {
            for(state_type p = 0; p < m_fsa.size(); p++) {
                ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(p);
                for(arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++) {
                    state_type temp = ait->getDest();
                    arc_type arc(*ait);
                    arc.setDest(p);
                    
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
            typename std::map<state_type, std::set<arc_type, ArcSorter> >::const_iterator cit
                = reversedArcs.find(p);
            if(cit != reversedArcs.end())
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
            for(arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++)
                arcs.insert(*ait);
            return arcs;
        }
        
      private:
        const FSA &m_fsa;
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
                if(m_fsa.isEndState(q))
                    return true;
            return false;
        }
        
        std::set<arc_type, ArcSorter> arcFn(State p) const {
            std::set<arc_type, ArcSorter> arcs;
            BOOST_FOREACH(state_type q, m_fsa.epsClosure(p)) {
                ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(q);
                for(arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++)
                    if(ait->getSymbol() != EPS) 
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
                if(m_fsa.isEndState(q))
                    return true;
            return false;
        }
        
        std::set<arc_type, ArcSorter> arcFn(state_type p) const {
            std::set<arc_type, ArcSorter> arcs;
            std::map<typename FSA::arc_type::symbol_type, arc_type> arcsMap;
            BOOST_FOREACH(typename FSA::state_type q, p) {
                ArcRange<typename FSA::arc_iterator_type> arcRange = m_fsa.getArcs(q);
                for(typename FSA::arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++) {
                    arc_type& arc = arcsMap[ait->getSymbol()];
                    arc.setSymbol(ait->getSymbol());
                    arc.getDest().insert(ait->getDest());                    
                }
            }
            
            typedef typename std::map<typename FSA::arc_type::symbol_type, arc_type>::value_type map_type;
            BOOST_FOREACH(map_type m, arcsMap)
                arcs.insert(m.second);
            
            return arcs;
        }
        
      private:
        const FSA &m_fsa;
    };
    
      
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
       
        while(!queue.empty()) {
            WrapperState s = queue.back();
            queue.pop_back();
            
            if(!mapper.count(s)) {
                State n = fsa.addState();
                mapper[s] = n;
            }
            
            if(wrapper.finalFn(s))
                fsa.setEndState(mapper[s]);
                
            BOOST_FOREACH( WrapperArc arc, wrapper.arcFn(s) ) {
                if(!mapper.count(arc.getDest())) {
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
    void reverse(FSA &fsa) {
        FSA temp;
        Reverser<FSA> rvr(fsa);
        traverse(rvr, temp);
        fsa.swap(temp);
    }
    
    template <typename FSA>
    void reachable(FSA &fsa) {
        FSA temp;
        traverse(Copier<FSA>(fsa), temp);
        fsa.swap(temp);
    }
    
    template <typename FSA>
    void prune(FSA &fsa) {
        reverse(fsa);
        reachable(fsa);
        reverse(fsa);
    }
    
    template <typename FSA>
    void epsRemove(FSA &fsa) {
        FSA temp;
        EpsRemover<FSA> epsrm(fsa);
        traverse(epsrm, temp);
        fsa.swap(temp);
    }
    
    template <typename FSA>
    void determinize(FSA &fsa) {
        FSA temp;
        Determinizer<FSA> dtr(fsa);
        traverse(dtr, temp);
        fsa.swap(temp);
    }
    
    template <typename FSA>
    void minimize(FSA &fsa) {
        epsRemove(fsa);
        reverse(fsa);
        determinize(fsa);
        reverse(fsa);
        determinize(fsa);
    }
    
    template <typename FSA1, typename FSA2>
    void unify(FSA1 &dst, FSA2 &src) {
        Copier<FSA2> copier(src);
        traverse(copier, dst);
    }
    
    template <typename FSA1, typename FSA2>
    void concatenate(FSA1 &dst, FSA2 &src) {
        std::set<typename FSA1::state_type> first_start  = dst.getStartStates();
        std::set<typename FSA1::state_type> first_end    = dst.getEndStates();
        
        Copier<FSA2> copier(src);
        traverse(copier, dst);
        
        std::set<typename FSA1::state_type> second_start = dst.getStartStates();
        
        BOOST_FOREACH(typename FSA1::state_type p, first_end) {
            BOOST_FOREACH(typename FSA1::state_type q, second_start) {
                if(!first_start.count(q)) {
                    dst.addArc(p, typename FSA1::arc_type(EPS, q));
                    dst.unsetStartState(q);
                }
            }
            dst.unsetEndState(p);
        }
    }
    
    template <typename FSA>
    void kleene_option(FSA &fsa) {
        std::set<typename FSA::state_type> &start  = fsa.getStartStates();
        std::set<typename FSA::state_type> &end    = fsa.getEndStates();
        
        BOOST_FOREACH(typename FSA::state_type p, start)
            BOOST_FOREACH(typename FSA::state_type q, end)
                fsa.addArc(p, typename FSA::arc_type(EPS, q));
    }

    template <typename FSA>
    void kleene_plus(FSA &fsa) {
        std::set<typename FSA::state_type> &start  = fsa.getStartStates();
        std::set<typename FSA::state_type> &end    = fsa.getEndStates();
        
        BOOST_FOREACH(typename FSA::state_type p, end)
            BOOST_FOREACH(typename FSA::state_type q, start)
                fsa.addArc(p, typename FSA::arc_type(EPS, q));
    }
    
    template <typename FSA>
    void kleene_star(FSA &fsa) {
        kleene_option(fsa);
        kleene_plus(fsa);
    }
}

#endif

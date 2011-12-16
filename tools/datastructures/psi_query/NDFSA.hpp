#ifndef NDFSA_HPP__
#define NDFSA_HPP__

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <boost/foreach.hpp>

#include "FSATypes.hpp"

namespace psi {
    
    template <typename StateT = State, typename ArcT = Arc<Symbol, State> >
    class NDFSA {
      protected:
        typedef std::set<ArcT, ArcSorter> Arcs;
        typedef std::vector<Arcs*> States;
    
      public:
        typedef StateT state_type;
        typedef ArcT arc_type;
        typedef typename ArcT::symbol_type symbol_type;
        typedef typename Arcs::iterator arc_iterator_type;
    
      protected:      
        States m_states;
        
        std::set<state_type> m_startStates;
        std::set<state_type> m_endStates;
               
        arc_iterator_type find(state_type, symbol_type) const;
        
      public:
        NDFSA();
        
        NDFSA(symbol_type &);
        
        template <typename InputIterator>
        NDFSA(InputIterator, InputIterator);
        
        ~NDFSA();
             
        std::set<state_type> delta(std::set<state_type>, symbol_type) const;
        std::set<state_type> deltaPrime(state_type, symbol_type) const;
        
        std::set<state_type> epsClosure(state_type) const;
             
        template <typename InputIterator>
        bool in(InputIterator, InputIterator);
        
        state_type addState(bool = false);
        
        const std::set<state_type>& getStartStates() const;
        bool isStartState(state_type) const;
        
        void setStartState(state_type);
        void unsetStartState(state_type);
        
        const std::set<state_type>& getEndStates() const;
        bool isEndState(state_type) const;
        
        void setEndState(state_type);
        void unsetEndState(state_type);
        
        void addArc(state_type, arc_type);
        const ArcRange<arc_iterator_type> getArcs(state_type) const;
        
        size_t size() const;
        void swap(NDFSA &);
        void print();
        
        NDFSA<state_type, arc_type>& operator=(const NDFSA<state_type, arc_type> &); 
        
    };

    template <typename StateT, typename ArcT>
    NDFSA<StateT, ArcT>::NDFSA() {}

    template <typename StateT, typename ArcT>
    NDFSA<StateT, ArcT>::NDFSA(typename ArcT::symbol_type &s) {
        StateT q0 = addState(true);
        StateT q1 = addState();
        
        addArc(q0, ArcT(q1, s));
    }

    template <typename StateT, typename ArcT>
    template <typename InputIterator>
    NDFSA<StateT, ArcT>::NDFSA(InputIterator it, InputIterator end) {
        StateT q0 = addState(true);        
        StateT p, q = q0;
        
        while(it != end) {
            p = q;
            q = addState();
        
            addArc(p, ArcT(*it, q));
            it++;
        }
        setEndState(q);    
    }

    template <typename StateT, typename ArcT>
    NDFSA<StateT, ArcT>::~NDFSA() {
        BOOST_FOREACH( Arcs* arcs, m_states )
            delete arcs;
    }
    
    template <typename StateT, typename ArcT>
    inline std::set<StateT> NDFSA<StateT, ArcT>::deltaPrime(StateT p, typename ArcT::symbol_type a) const {
        if(m_states.size() < p)
            return std::set<StateT>();
    
        std::set<StateT> destinations;
        arc_iterator_type arc = find(p, a);
        while(arc != m_states[p]->end() && arc->getSymbol() == a) {
            destinations.insert(arc->getDest());
            arc++;
        }        
        return destinations;
    }
    
    template <typename StateT, typename ArcT>
    inline std::set<StateT> NDFSA<StateT, ArcT>::delta(std::set<StateT> pSet, typename ArcT::symbol_type a) const {
        std::set<StateT> destinations;
        BOOST_FOREACH(StateT p, pSet) {
            std::set<StateT> destPrime = deltaPrime(p, a); 
            destinations.insert(destPrime.begin(), destPrime.end());
        }
        return destinations;
    }
    
    template <typename StateT, typename ArcT>
    inline typename NDFSA<StateT, ArcT>::arc_iterator_type
    NDFSA<StateT, ArcT>::find(StateT p, typename ArcT::symbol_type a) const {
        ArcRange<arc_iterator_type> r = getArcs(p);
    
        ArcT test(a, -1);
        return std::lower_bound(r.first, r.second, test, ArcSorter());
    }
    
    template <typename StateT, typename ArcT>
    template <typename InputIterator>
    bool NDFSA<StateT, ArcT>::in(InputIterator it, InputIterator end) {
        std::set<StateT> current_states = getStartStates();
        
        while(it != end) {
            std::set<StateT> next_states = delta(current_states, *it);    
            if(next_states.size())
                current_states = next_states;
            else
                return false;
            it++;
        }
        BOOST_FOREACH(StateT p, current_states)
            if(isEndState(p))
                return true;
        return false;
    }

    template <typename StateT, typename ArcT>
    StateT NDFSA<StateT, ArcT>::addState(bool isStartState) {
        m_states.push_back(new Arcs());
        if(isStartState)
            m_startStates.insert(state_type(m_states.size()-1));
    
        return m_states.size()-1;
    }
    
    template <typename StateT, typename ArcT>
    const std::set<StateT>& NDFSA<StateT, ArcT>::getStartStates() const {
        return m_startStates;
    }   

    template <typename StateT, typename ArcT>
    bool NDFSA<StateT, ArcT>::isStartState(StateT state) const {
        return m_startStates.count(state);
    }
    
    template <typename StateT, typename ArcT>
    void NDFSA<StateT, ArcT>::setStartState(StateT state) {
        m_startStates.insert(state);
    }
    
    template <typename StateT, typename ArcT>
    void NDFSA<StateT, ArcT>::unsetStartState(StateT state) {
        m_startStates.erase(state);
    }

    template <typename StateT, typename ArcT>
    const std::set<StateT>& NDFSA<StateT, ArcT>::getEndStates() const {
        return m_endStates;
    }   

    template <typename StateT, typename ArcT>
    bool NDFSA<StateT, ArcT>::isEndState(StateT state) const {
        return m_endStates.count(state);
    }
    
    template <typename StateT, typename ArcT>
    void NDFSA<StateT, ArcT>::setEndState(StateT state) {
        m_endStates.insert(state);
    }
    
    template <typename StateT, typename ArcT>
    void NDFSA<StateT, ArcT>::unsetEndState(StateT state) {
        m_endStates.erase(state);
    }
   
    template <typename StateT, typename ArcT>
    void NDFSA<StateT, ArcT>::addArc(StateT state, ArcT arc) {
        m_states[state]->insert(arc);
    }
    
    template <typename StateT, typename ArcT>
    const ArcRange<typename NDFSA<StateT, ArcT>::arc_iterator_type>
    NDFSA<StateT, ArcT>::getArcs(StateT p) const {
        return ArcRange<arc_iterator_type>(
                m_states[p]->begin(),
                m_states[p]->end()
            );
    }
    
    template <typename StateT, typename ArcT>
    size_t NDFSA<StateT, ArcT>::size() const {
        return m_states.size();
    }

    template <typename StateT, typename ArcT>
    void NDFSA<StateT, ArcT>::swap(NDFSA<StateT, ArcT> &fsa) {
        m_states.swap(fsa.m_states);
        m_startStates.swap(fsa.m_startStates);
        m_endStates.swap(fsa.m_endStates);
    }
    
    template <typename StateT, typename ArcT>
    void NDFSA<StateT, ArcT>::print() {
        for(int i = 0; i < m_states.size(); i++) {
           ArcRange<arc_iterator_type> r = getArcs(i);
           for(arc_iterator_type it = r.first; it != r.second; it++) {
               std::cout << i << "\t" << it->getDest() << "\t" << it->getSymbol() << std::endl;
           }
           if(isEndState(i)) {
               std::cout << i << std::endl;
           }
        }
    }
    
    template <typename StateT, typename ArcT>
    std::set<StateT> NDFSA<StateT, ArcT>::epsClosure(StateT p) const {
        std::set<StateT> closure;
        std::vector<StateT> queue;
        
        closure.insert(p);
        queue.push_back(p);
        while(!queue.empty()) {
            StateT s = queue.back();
            queue.pop_back();
            
            ArcRange<arc_iterator_type> arcs = getArcs(s);
            for(arc_iterator_type ait = arcs.first; ait != arcs.second; ait++) {
                if(ait->getSymbol() == EPS && !closure.count(ait->getDest())) {
                    closure.insert(ait->getDest());
                    queue.push_back(ait->getDest());
                }
            }
        }    
        return closure;
    }
    
    template <typename StateT, typename ArcT>
    NDFSA<StateT, ArcT>& NDFSA<StateT, ArcT>::operator=(const NDFSA<StateT, ArcT> &a) {
        BOOST_FOREACH(Arcs* arcs, a.m_states) 
            m_states.push_back(new Arcs(*arcs));
        
        m_endStates = a.m_endStates;
        m_startStates = a.m_startStates;
        
        return *this;
    }

}


#endif

#ifndef DFSA_HPP__
#define DFSA_HPP__

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <boost/foreach.hpp>

#include "DFSATypes.hpp"

namespace psi {
    
    template <typename StateT = State, typename ArcT = Arc<Symbol, State> >
    class DFSA {
      private:
        typedef std::vector<ArcT> Arcs;
        typedef std::vector<Arcs*> States;
    
      public:
        typedef StateT state_type;
        typedef ArcT arc_type;
        typedef typename ArcT::symbol_type symbol_type;
        typedef typename Arcs::iterator arc_iterator_type;
    
      private:      
        States m_states;
        
        std::set<state_type> m_startStates;
        std::set<state_type> m_endStates;
               
        inline state_type delta(state_type, symbol_type);
        inline arc_iterator_type find(state_type, symbol_type);
        inline arc_iterator_type linsearch(arc_iterator_type, arc_iterator_type, symbol_type);
        inline arc_iterator_type binsearch(arc_iterator_type, arc_iterator_type, symbol_type);
        
      public:
        DFSA();
        ~DFSA();
             
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
        void swap(DFSA &);
        void print();
    };

    template <typename StateT, typename ArcT>
    DFSA<StateT, ArcT>::DFSA() {}

    template <typename StateT, typename ArcT>
    DFSA<StateT, ArcT>::~DFSA() {
        BOOST_FOREACH( Arcs* arcs, m_states )
            delete arcs;
    }
    
    template <typename StateT, typename ArcT>
    inline StateT DFSA<StateT, ArcT>::delta(StateT p, typename ArcT::symbol_type a) {
        if(m_states.size() < p)
            return -1;
    
        arc_iterator_type arc = find(p, a);
        if(arc == m_states[p]->end())
            return -1;
        else
            return arc->getDest(); 
    }
    
    template <typename StateT, typename ArcT>
    inline typename DFSA<StateT, ArcT>::arc_iterator_type
    DFSA<StateT, ArcT>::find(StateT p, typename ArcT::symbol_type a) {
        ArcRange<arc_iterator_type> r = getArcs(p);
    
        ArcT test(a, -1);
        return std::lower_bound(r.first, r.second, test, ArcSorter());
    }
    
    template <typename StateT, typename ArcT>
    template <typename InputIterator>
    bool DFSA<StateT, ArcT>::in(InputIterator it, InputIterator end) {
        StateT current_state = 0;
        while(it != end) {
            StateT next_state = delta(current_state, *it);    
            if(next_state != -1)
                current_state = next_state;
            else
                return false;
            it++;
        }
        return isEndState(current_state);
    }

    template <typename StateT, typename ArcT>
    StateT DFSA<StateT, ArcT>::addState(bool isStartState) {
        m_states.push_back(new Arcs());
        if(isStartState)
            m_startStates.insert(state_type(m_states.size()-1));
    
        return m_states.size()-1;
    }
    
    template <typename StateT, typename ArcT>
    const std::set<StateT>& DFSA<StateT, ArcT>::getStartStates() const {
        return m_startStates;
    }   

    template <typename StateT, typename ArcT>
    bool DFSA<StateT, ArcT>::isStartState(StateT state) const {
        return m_startStates.count(state);
    }
    
    template <typename StateT, typename ArcT>
    void DFSA<StateT, ArcT>::setStartState(StateT state) {
        m_startStates.insert(state);
    }
    
    template <typename StateT, typename ArcT>
    void DFSA<StateT, ArcT>::unsetStartState(StateT state) {
        m_startStates.erase(state);
    }

    template <typename StateT, typename ArcT>
    const std::set<StateT>& DFSA<StateT, ArcT>::getEndStates() const {
        return m_endStates;
    }   

    template <typename StateT, typename ArcT>
    bool DFSA<StateT, ArcT>::isEndState(StateT state) const {
        return m_endStates.count(state);
    }
    
    template <typename StateT, typename ArcT>
    void DFSA<StateT, ArcT>::setEndState(StateT state) {
        m_endStates.insert(state);
    }
    
    template <typename StateT, typename ArcT>
    void DFSA<StateT, ArcT>::unsetEndState(StateT state) {
        m_endStates.erase(state);
    }
   
    template <typename StateT, typename ArcT>
    void DFSA<StateT, ArcT>::addArc(StateT state, ArcT arc) {
        m_states[state]->push_back(arc);
        std::sort(m_states[state]->begin(), m_states[state]->end(), ArcSorter());
    }
    
    template <typename StateT, typename ArcT>
    const ArcRange<typename DFSA<StateT, ArcT>::arc_iterator_type>
    DFSA<StateT, ArcT>::getArcs(StateT p) const {
        return ArcRange<arc_iterator_type>(
                m_states[p]->begin(),
                m_states[p]->end()
            );
    }
    
    template <typename StateT, typename ArcT>
    size_t DFSA<StateT, ArcT>::size() const {
        return m_states.size();
    }

    template <typename StateT, typename ArcT>
    void DFSA<StateT, ArcT>::swap(DFSA<StateT, ArcT> &fsa) {
        m_states.swap(fsa.m_states);
        m_startStates.swap(fsa.m_startStates);
        m_endStates.swap(fsa.m_endStates);
    }
    
    template <typename StateT, typename ArcT>
    void DFSA<StateT, ArcT>::print() {
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

}


#endif

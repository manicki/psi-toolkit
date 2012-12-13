#ifndef SIMPLEDAG_HPP__
#define SIMPLEDAG_HPP__

#include <vector>
#include <map>
#include <set>
#include <limits>
#include <algorithm>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/dynamic_bitset.hpp>

#include "RuleTypes.hpp"

namespace poleng
{

namespace bonsai
{

namespace rules {
      
    class SimpleDAG { 
      private:
        typedef boost::dynamic_bitset<> BSet;
        
        struct ArcSorter {
            bool operator()(Arc lhs, Arc rhs) const { return lhs.a < rhs.a; }
        } as;
    
        typedef std::vector<Arc> Arcs;
        typedef boost::shared_ptr<Arcs> ArcsPtr;
        typedef std::vector<ArcsPtr> States;
        
        States states;
        
        std::set<State> end_states;
        
        BSet starts;
        
        inline State delta(State, Symbol);
        inline ArcIt linsearch(ArcIt, ArcIt, Symbol);
        inline ArcIt binsearch(ArcIt, ArcIt, Symbol);
        inline ArcIt find(State, Symbol);
    
        void wordlist_rec(State, StateLanguage&,  std::set<State>&);
        
    public:  
        Range row(State);
        void set_end_state(State);
        bool is_end_state(State);
        State new_state(bool);
        void delete_state(State);
        void new_arc(State, State, Symbol, Weight);
        void print();
        void print_with_weights();
        
        int size() {
            return states.size();
        }
        
        SimpleDAG();
        
        void reverse();
        void determinize();
        void minimize();
        void nd_union(SimpleDAG &);
        void det_union(SimpleDAG &);
            
        bool in(Word &);
        int hash(Word &w);
        
        WordList wordlist();
        
        bool prefix(Word &);
        State common_prefix(Word &, int &);
    };

}

}

}

#endif

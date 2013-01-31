#include "SimpleDAG.hpp"

namespace poleng
{

namespace bonsai
{

using namespace rules;


// ########### Konstruktory ############

SimpleDAG::SimpleDAG() {}


// ########### Funkcja sprawdzająca przynależność do automatu, funkcja haszująca, funkcja dehaszująca ############

bool SimpleDAG::in(Word &w) { // sprawdza czy dany wyraz (ciąg liczb) jest elementem języka automatu   
    State current_state = 0;
    for(size_t i = 0; i < w.size(); i++) {
        State next_state = delta(current_state, w[i]);    
        if(next_state != -1) {
            current_state = next_state;
        }
        else {
            return false;
        }
    }
    return is_end_state(current_state);
}

int SimpleDAG::hash(Word &w) {   
    int index = 0;
    State current_state = 0;
    for(size_t i = 0; i < w.size(); i++) {
        ArcIt arc = find(current_state, w[i]);
        if(arc != NULL) {
            index += arc->w;
            current_state = arc->q;
            if(is_end_state(current_state))
                index++;
        }
        else return -1;
    }
    if(is_end_state(current_state)) {
        return index;
    }
    else {
        return -1;
    }
}

WordList SimpleDAG::wordlist() {
    StateLanguage sl;
    sl.resize(size(), WordList(1, WordIndex(Word(), 0))); 
    
    std::set<State> visited;
    wordlist_rec(0, sl, visited);
    return sl[0];
}

void SimpleDAG::wordlist_rec(State q, StateLanguage &sl, std::set<State> &visited) {
    Range r = row(q);
    
    if(visited.count(q))
        return;
    
    visited.insert(q);
    
    for(ArcIt ait = r.first; ait != r.second; ait++) {
        wordlist_rec(ait->q, sl, visited);
     
        for(WordList::iterator it = sl[ait->q].begin(); it != sl[ait->q].end(); it++) {
            WordIndex wi(*it);
            wi.get<0>().insert(wi.get<0>().begin(), ait->a);
            wi.get<1>() += ait->w;
            if(is_end_state(ait->q))
                wi.get<1>()++;
            
            sl[q].push_back(wi);
        }
    
    }
}

bool SimpleDAG::prefix(Word &w) {    
    State current_state = 0;
    for(size_t i = 0; i < w.size(); i++) {
        State next_state = delta(current_state, w[i]);    
        if(next_state != -1) {
            current_state = next_state;
        }
        else {
            return false;
        }
    }
    return true;
}

State SimpleDAG::common_prefix(Word &w, int &l) {    
    State current_state = 0;
    for(size_t i = 0; i < w.size(); i++) {
        State next_state = delta(current_state, w[i]);    
        if(next_state != -1) {
            current_state = next_state;
        }
        else {
            return false;
        }
        l = i;
    }
    return current_state;
}

// ########### Funkcje do nawigacji po automacie ###########

inline State SimpleDAG::delta(State qs, Symbol as) {  // funkcja przejścia
    if(states.empty())
        return -1;
    ArcIt arc = find(qs,as);
    if(arc == NULL)
        return -1;
    else
        return arc->q; 
}

inline ArcIt SimpleDAG::find(State i, Symbol j) {
    ArcIt arc = NULL;
    
    Range r = row(i);
    int dist = (r.second - r.first);

    if(dist > 10)
        return arc = binsearch(r.first, r.second-1, j);
    else if(dist > 0)
        return arc = linsearch(r.first, r.second-1, j);
    
    return arc; 
}

Range SimpleDAG::row(State i) {  
    if((size_t)i < states.size()) {    
        return Range(&(*(states[i]->begin())), &(*(states[i]->end()))); 
    }
    else 
        return Range(NULL, NULL);
}

inline ArcIt SimpleDAG::linsearch(ArcIt start, ArcIt end, Symbol value) {
    for(ArcIt it = start; it <= end; it++) {
        if(it->a == value) {
            return it;
        }
    }
    return NULL;
}

inline ArcIt SimpleDAG::binsearch(ArcIt start, ArcIt end, Symbol value) {
    ArcIt L = start;
    ArcIt R = end; 
    while (L <= R) {
        ArcIt m = L + ((R - L)/2);
        if (m->a < value)
            L = m + 1;
        else if (m->a > value)
            R = m - 1;
        else 
            return m;
    }
    return NULL;  
}

// ########### Manipulacje bitami do zaznaczanie stanów końcowych ###########

void SimpleDAG::set_end_state(State p) {             // funkcja, która ustawia bit stanu końcowego
    end_states.insert(p);
}

bool SimpleDAG::is_end_state(State p) {               // funkcja, która sprawdza bit stanu końcowego 
    return (end_states.count(p) > 0);
}

State SimpleDAG::new_state(bool start = false) {
    states.push_back(ArcsPtr( new Arcs() ));
    starts.resize(states.size());
    
    if(start)
        starts[states.size()-1] = 1;
    
    return states.size()-1;
}

void SimpleDAG::delete_state(State p) {
    states.erase(states.begin() + p);
}

void SimpleDAG::new_arc(State start, State end, Symbol a, Weight w) {
    Arc arc;
    arc.a = a;
    arc.q = end;
    arc.w = w;
    
    states[start]->push_back(arc);
    std::sort(states[start]->begin(), states[start]->end(), as);
}

void SimpleDAG::print() {
    //std::cerr << "start: " << starts << std::endl;
    //std::cerr << starts.size() << " - " << states.size() << std::endl;
    
    //std::cout << "Size: " << states.size() << std::endl;
    for(size_t i = 0; i < states.size(); i++) {
        Range r = row(i);
        for(ArcIt it = r.first; it != r.second; it++) {
            std::cout << i << "\t" << it->q << "\t" << it->a << std::endl;
        }
        if(is_end_state(i)) {
            std::cout << i << std::endl;
        }
    }
}

void SimpleDAG::print_with_weights() {
    for(size_t i = 0; i < states.size(); i++) {
        Range r = row(i);
        for(ArcIt it = r.first; it != r.second; it++) {
            std::cout << i << "\t" << it->q << "\t" << it->a << "\t" << it->w << std::endl;
        }
        if(is_end_state(i)) {
            std::cout << i << std::endl;
        }
    }
}


void SimpleDAG::reverse() {
    SimpleDAG ndag;
    std::map<State, State> mapper;
    
    for(size_t i=0; i < states.size(); i++) {
        State s = ndag.new_state();
        if(is_end_state(i)) {
            ndag.starts[s] = 1;
        }
        if(starts[i] == 1) {
            ndag.set_end_state(s);
        }
    }
    for(size_t i = 0; i < ndag.states.size(); i++) {
        Range r = row(i);
        for(ArcIt ait = r.first; ait != r.second; ait++) {
            ndag.new_arc(ait->q, i, ait->a, ait->w);
        }
    }
    
    starts.swap(ndag.starts);
    states.swap(ndag.states);
    end_states.swap(ndag.end_states);
}

void SimpleDAG::determinize() {
    typedef boost::dynamic_bitset<> BSet;
    typedef std::map<Symbol, BSet> ArcSet;
    
    SimpleDAG ndag;
    std::map<BSet, State> mapper;
        
    std::set<BSet> nstates;
    int size = states.size();
    
    BSet start = starts;
    mapper[start] = ndag.new_state(true);
    //nstates.insert(start);
    
    std::vector<BSet> queue;
    queue.push_back(start);
    
    std::map<BSet, ArcSet> newfsa;
    
    while(not queue.empty()) {
        BSet ns = queue.back();
        queue.pop_back();
        
        ArcSet narcs;
        State s = ns.find_first();
        
        while((size_t)s != BSet::npos) {
            Range arcs = row(s);
            for(ArcIt ait = arcs.first; ait != arcs.second; ait++) {
                if(narcs.count(ait->a) == 0) {
                    BSet tempset(size); 
                    narcs.insert(std::make_pair(ait->a, tempset));
                }
                narcs[ait->a][ait->q] = 1;
            }
            s = ns.find_next(s);
        }
        
        for(std::map<Symbol, BSet>::iterator it = narcs.begin(); it != narcs.end(); it++) {
            if(nstates.count(it->second) == 0) {
                nstates.insert(it->second);
                queue.push_back(it->second);
            }
        }
        
        newfsa[ns] = narcs;
    }
    
    for(std::map<BSet, ArcSet>::iterator it = newfsa.begin(); it != newfsa.end(); it++) {
        
        State ndag_s;
        if(mapper.count(it->first) == 0) {
            ndag_s = ndag.new_state();
            mapper[it->first] = ndag_s;
        }
        else {
            ndag_s = mapper[it->first];
        }
        
        State s = it->first.find_first(); 
        while((size_t)s != BSet::npos) {
            if(is_end_state(s)) {
                ndag.set_end_state(ndag_s);
            }
            //if(starts[s]==1) {
            //    ndag.starts[ndag_s] = 1;
            //}
            s = it->first.find_next(s);
        }
        
        for(ArcSet::iterator ait = it->second.begin(); ait != it->second.end(); ait++) {
            State ndag_t;
            if(mapper.count(ait->second) == 0) {
                ndag_t = ndag.new_state();
                mapper[ait->second] = ndag_t;  
            }
            else {
                ndag_t = mapper[ait->second];
            }
            ndag.new_arc(ndag_s, ndag_t, ait->first, 0);
        }
    }
    
    starts.swap(ndag.starts);
    states.swap(ndag.states);
    end_states.swap(ndag.end_states);
}

void SimpleDAG::minimize() {
    reverse();
    determinize();
    reverse();
    determinize();
}

void SimpleDAG::det_union(SimpleDAG &sdu) {
    nd_union(sdu);
    minimize();
}

void SimpleDAG::nd_union(SimpleDAG &sdu) {
    State act;
    
    std::map<State, State> mapper;
    for(size_t i=0; i<sdu.states.size(); i++) {
        if(mapper.count(i) == 0) {
            State n;
            
            if(sdu.starts[i] == 1)
                n = new_state(true);
            else
                n = new_state();
            
            mapper[i] = n;
        }
        act = mapper[i];
        
        if(sdu.is_end_state(i))
            set_end_state(act);
            
        if(sdu.starts[i] == 1) {
            starts[act] = 1;
        }
        
        Range r = sdu.row(i);
        for(ArcIt ait = r.first; ait != r.second; ait++) {
            
            if(mapper.count(ait->q) == 0) {
                State n = new_state();
                mapper[ait->q] = n;
            }
            new_arc(act, mapper[ait->q], ait->a, ait->w);
        }
    }
}

}

}

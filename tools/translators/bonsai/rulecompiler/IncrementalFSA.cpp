#include "IncrementalFSA.hpp"

namespace poleng
{

namespace bonsai
{

IncrementalFSA::IncrementalFSA() : verbose(false), states(0) {
    add_new_state();
}

IncrementalFSA::IncrementalFSA(int register_no) : verbose(false), states(0) {
    add_new_state();
}

void IncrementalFSA::add_word(Word &w) {
    if(verbose) {
	std::cerr << "Adding word: ";
	for(Word::iterator it = w.begin(); it != w.end(); it++)
	    std::cerr << *it << " ";
	std::cerr << std::endl;
    }
    
    Word::iterator common_prefix_end = w.begin();
    int last_state = delta_prefix(common_prefix_end, w.end());
    if(verbose) std::cerr << "Common prefix size: " << std::distance(w.begin(), common_prefix_end) << std::endl;
    if(verbose) std::cerr << "Last state: " << last_state << std::endl;
    
    Word::iterator suffix_start;
    if(last_state == 0)
	suffix_start = common_prefix_end;
    else
	suffix_start = common_prefix_end; //+1;
    
    if(has_children(last_state)) {
	replace_or_register(last_state);
    }
    
    add_branch(last_state, suffix_start, w.end());
    purge();
}

void IncrementalFSA::replace_or_register(int state) {
    if(verbose) std::cerr << "Entering replace_or_register for state: " << state << std::endl;
    int child = last_child(state);
    
    if(has_children(child)) {
	replace_or_register(child);
    }
    
    if(verbose) std::cerr << "Creating signature for: " << child << std::endl;
    StateSignature child_sig = make_sig(child);
    if(verbose) std::cerr << "Signature state: " << child_sig.state << " - length: " << child_sig.sig_length << std::endl;
    if(verbose) std::cerr << "Searching for equivalent state for: " << child << std::endl;
    
    if(sig.count(child_sig) > 0) {
	int equiv = sig.find(child_sig)->state;
	if(verbose) std::cerr << "Found equivalent states: " << child << " and " << equiv << std::endl;
    
	last_child(state) = equiv;
	delete_state(child);
    }
    else {
	if(verbose) std::cerr << "No equivalent state found for: " << child << std::endl;
    
	sig.insert(child_sig);
    }
}

StateSignature IncrementalFSA::make_sig(int state) {
    Transitions::iterator start = state_table[state].begin();
    Transitions::iterator end = state_table[state].end();
    
    int length = std::distance(start,end);
    
    StateSignature s(length);
    s.state = state;
    
    
    s.signature[0] = end_states.count(state);
    
    int i = 1;
    while(start != end) {
	s.signature[i++] = start->first;
	s.signature[i++] = start->second;
	start++;
    }
    
    return s;
}

int& IncrementalFSA::last_child(int state) {
    if(verbose) std::cerr << "Getting last child for " << state << " : ";
    int last = state_table[state].rbegin()->second;
    if(verbose) std::cerr << last << std::endl;
    return state_table[state].rbegin()->second;
}

bool IncrementalFSA::has_children(int state) {
    if(verbose) std::cerr << "Checking whether " << state << " has children: "; 
    bool answer = (state_table[state].size() > 0);
    if(verbose) std::cerr << answer << std::endl;
    return answer;
}

int IncrementalFSA::add_new_state() {
    int new_state = states;
    states++;
    state_table[new_state] = Transitions();
    return new_state;
}

void IncrementalFSA::add_new_transition(int from, int to, int symbol) {
    if(state_table.count(from) == 0) {
	std::cerr << "No state '" << from << "'" << std::endl;
	exit(1);
    }
    if(state_table.count(to) == 0) {
	std::cerr << "No state '" << to << "'" << std::endl;
	exit(1);            
    }
    state_table[from][symbol] = to;
    if(verbose) std::cerr << "Adding arc : " << from << " -- " << symbol << " --> " << to << std::endl;
}

void IncrementalFSA::add_branch(int start_state, Word::iterator start, Word::iterator end) {
    if(verbose) std::cerr << "Adding branch" << std::endl;
    last_added.clear();
    int current = start_state;
    for(Word::iterator it = start; it != end; it++) {
	int next_state = add_new_state();
	last_added.insert(next_state);
	add_new_transition(current, next_state, *it);
	current = next_state;
    }
    set_end_state(current);
}

void IncrementalFSA::delete_state(int state) {
    if(state_table.count(state)) {
	state_table.erase(state);
	if(end_states.count(state)) end_states.erase(state);        
	states--;
    }
}

int IncrementalFSA::delta(Word::iterator start, Word::iterator end) {
    last_traversed.clear();
    int current_state = 0;
    for(Word::iterator it = start; it != end; it++) {
	last_traversed.insert(current_state);
	if(state_table.count(current_state) > 0 and state_table[current_state].count(*it) >0) {
	    current_state = state_table[current_state][*it];
	}
	else {
	    return -1;
	}
    }
    last_traversed.insert(current_state);
    if(is_end_state(current_state))
	return current_state;
    else
	return -1;
}

int IncrementalFSA::delta_prefix(Word::iterator& current, Word::iterator end) {
    last_traversed.clear();
    int current_state = 0;
    
    while(current != end) {
	last_traversed.insert(current_state);
	if(state_table.count(current_state) > 0 and state_table[current_state].count(*current) >0) {
	    current_state = state_table[current_state][*current];
	}
	else {
	    return current_state;
	}
	current++;
    }
    last_traversed.insert(current_state);
}
 

bool IncrementalFSA::is_end_state(int state) {
    return end_states.count(state) > 0;
}

void IncrementalFSA::set_end_state(int state) {
    if(verbose) std::cerr << "Adding end state: " << state << std::endl;
    end_states.insert(state);
}

void IncrementalFSA::purge() {
    if(last_added.size() > 0) {
	int min = *(last_added.begin());
	std::set<int> for_deletion;
	for(TransitionTable::iterator it1 = state_table.begin(); it1 != state_table.end() and it1->first < min; it1++) {
	    if(last_traversed.count(it1->first) == 0) {
		for(Transitions::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
		  std::cout << it1->first << "\t" << it2->second << "\t" << it2->first << std::endl;  
		}
		for_deletion.insert(it1->first);
	    }
	}
	
	if(for_deletion.size() > 0) {
	    for(std::set<int>::iterator it = for_deletion.begin(); it != for_deletion.end(); it++) {
		state_table.erase(*it);
		if(verbose) std::cerr << "Erasing " << *it << std::endl;
		if(end_states.count(*it)) {
		    std::cout << *it << std::endl;
		    end_states.erase(*it);
		}
	    }
	}
    }
}

void IncrementalFSA::print_rest() {
    for(TransitionTable::iterator it1 = state_table.begin(); it1 != state_table.end(); it1++) {
	for(Transitions::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
	    std::cout << it1->first << "\t" << it2->second << "\t" << it2->first << std::endl;  
	}
    }
    for(std::set<int>::iterator it = end_states.begin(); it != end_states.end(); it++) {
	std::cout << *it << std::endl;
    }
}

void IncrementalFSA::finalize() {
    replace_or_register(0);
    print_rest();
}

}

}

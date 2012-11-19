#ifndef INCREMENTALFSA_H__
#define INCREMENTALFSA_H__

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "Register.hpp"

namespace poleng
{
namespace bonsai
{

typedef std::map<int, int> Transitions;
typedef std::map<int, Transitions> TransitionTable;

typedef std::vector<int> Word;

class IncrementalFSA {
  private:
    TransitionTable state_table;
    std::set<int> end_states;
    Signatures sig;
    bool verbose;
    int states;
    
    std::set<int> last_traversed;
    std::set<int> last_added;
    
    void replace_or_register(int);
    StateSignature make_sig(int);

    int& last_child(int);
    bool has_children(int);

    int add_new_state();
    void add_new_transition(int, int, int);
    void add_branch(int, Word::iterator, Word::iterator);
    void delete_state(int);
    
    int delta(Word::iterator, Word::iterator);
    int delta_prefix(Word::iterator&, Word::iterator);
     
    bool is_end_state(int);
    void set_end_state(int);

    void purge();
    void print_rest();
    
  public:
    IncrementalFSA();
    IncrementalFSA(int);
    ~IncrementalFSA() {    }
    void add_word(Word &);
    void finalize();
    
    int size() {
	int c = 0;
	for(TransitionTable::iterator it = state_table.begin(); it != state_table.end(); it++)
	    c += it->second.size();
	return c;
    }
};

}

}

#endif

#ifndef LMCONTAINER_H__
#define LMCONTAINER_H__

#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>

#include <boost/shared_ptr.hpp>

#include "lmtable.h"
#include "TransferTypes.hpp"

namespace poleng
{

namespace bonsai
{

typedef std::pair<int,int> Link;
typedef std::set<std::pair<int,int> > LinkPoints;    

class LanguageModel {
  public:
    LanguageModel(std::string);
    
    int get_n();
    void clear_memory();
    double get_cost(SListPtr&, int);
    double get_cost(SListPtr&, LinkPoints&, int);
    double get_front_cost(SListPtr&, int i);
    double get_plain_cost(SListPtr&, int i);
    double get_terminal_cost(SListPtr&, int i);
    void set_lc(bool);
        
  private:
    lmtable* lm;
    ngram* ng;
    int n;
    bool lc;
};

typedef boost::shared_ptr<LanguageModel> LanguageModelPtr;
typedef std::vector<LanguageModelPtr> LanguageModelVector;

class LmContainer {
  public:
    LmContainer();
    
    void clear_memory();
    
    double get_cost(SListPtr&, Floats&);
    double get_cost(SListPtr&, Floats&, LinkPoints&);
    
    double get_front_cost(SListPtr&);
    std::string get_plain_cost(SListPtr&);
    
    double get_terminal_cost(SListPtr&);
    
    void add_lm(LanguageModelPtr);
    int size();
    LanguageModelPtr& at(int i) {
        return lms.at(i);
    }
  
    static void set_tm_weights(Floats &tm_weights_) { tm_weights = tm_weights_; }
    static void set_lm_weights(Floats &lm_weights_) { lm_weights = lm_weights_; }
    static void set_rs_weights(Floats &rs_weights_) { rs_weights = rs_weights_; }
    static void set_word_penalty_weight(double word_penalty_weight_) { word_penalty_weight = word_penalty_weight_; }
    
  private:
    LanguageModelVector lms;
    
    static Floats tm_weights;
    static Floats lm_weights;
    static Floats rs_weights;
    static double word_penalty_weight;

};

typedef boost::shared_ptr<LmContainer> LmContainerPtr;

}

}

#endif

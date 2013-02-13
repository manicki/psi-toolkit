#ifndef TRANSLATION_H__
#define TRANSLATION_H__

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <boost/shared_ptr.hpp>

#include "TransferTypes.hpp"
#include "Transformation.hpp"
#include "LmContainer.hpp"

#include "lattice.hpp"

namespace poleng
{

namespace bonsai
{

class Translation;
typedef boost::shared_ptr<Translation> TranslationPtr;
typedef std::map<Symbol, TranslationPtr, SymbolSorterMap> TranslationNodes;

class Translation {
  private:
    
    void substitute(TransformationPtr&, TranslationNodes&);
    
    int my_id;
    static int id;
    
    TransformationPtr parent_transformation;
    TranslationNodes  node_translations;
 
    LmContainerPtr lmc;
    
    
    bool top;
  
    SListPtr translation;
    Floats unweighted_costs;
    double cost;
    
    static int verbosity;
    static bool pedantry;
  
    double lm_heuristic;
    
    static Floats tm_weights;
    static Floats lm_weights;
    static Floats rs_weights;
    static double word_penalty_weight;
        
  public:
    Translation(TransformationPtr, TranslationNodes, LmContainerPtr, bool);
    int get_id();
    double get_cost();
    double get_lm_heuristic();
    Floats& get_unweighted();
    std::string str();
    const char* c_str() { return str().c_str(); }
    std::string nice();
    std::string mert(int i);
    std::string back_track(int);
    std::string dump_to_perl();
    Lattice::EdgeDescriptor annotateLattice(Lattice&, std::map<Symbol, Lattice::EdgeDescriptor, SymbolSorterMap2>&, std::map<int, Lattice::EdgeDescriptor>&);

    AlignmentPtr reconstruct_alignment(); 
    void reconstruct_alignment_rec(AlignmentPtr&, int&);

    TransformationPtr get_parent_transformation() { return parent_transformation; }
    SListPtr get_translation() { return translation; }
    TranslationPtr get_child_translation(Symbol sym) { return node_translations[sym]; }
    
    
    static void set_verbosity(int);
    static void set_pedantry(bool);
    
    static void set_tm_weights(Floats &tm_weights_) { tm_weights = tm_weights_; }
    static void set_lm_weights(Floats &lm_weights_) { lm_weights = lm_weights_; }
    static void set_rs_weights(Floats &rs_weights_) { rs_weights = rs_weights_; }
    static void set_word_penalty_weight(double word_penalty_weight_) { word_penalty_weight = word_penalty_weight_; }

};


struct TranslationSorter {
    bool operator()(const TranslationPtr &t1, const TranslationPtr &t2) const {
        return t1->get_cost() + t1->get_lm_heuristic() < t2->get_cost() + t2->get_lm_heuristic() and t1->str() != t2->str();
    }
};


typedef std::set<TranslationPtr, TranslationSorter> TranslationQueue;
typedef boost::shared_ptr<TranslationQueue> TranslationQueuePtr;
typedef std::vector<TranslationQueuePtr> TranslationNodesCandidates;
typedef boost::shared_ptr<TranslationNodesCandidates> TranslationNodesCandidatesPtr;

}

}

#endif

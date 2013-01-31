#ifndef RULESET_HPP__
#define RULESET_HPP__

#include <sstream>
#include <queue>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>

#include "lattice.hpp"
#include "lattice_wrapper.hpp"

#include "TransferTypes.hpp"
#include "Transformation.hpp"

#include "CompressedDAG.hpp"
#include "SimpleDAG.hpp"
#include "HuffedWords.hpp"
#include "MonotonicVector.hpp"
#include "StringVector.hpp"

#include "LmContainer.hpp"

namespace poleng
{

namespace bonsai
{

class RuleSet {
  public:
    RuleSet(std::string, int, int, int, LmContainerPtr);
    EdgeTransformationsPtr get_edge_transformations(Lattice&,
                                                    Lattice::VertexDescriptor,
                                                    Lattice::VertexDescriptor);
    void set_verbosity(int);    
    void set_max_transformations_per_hyperedge(int);    
    void set_max_hyperedges_per_nonterminal(int);    
    void set_max_transformation_factor(double);

    static void set_tm_weights(Floats &tm_weights_) { tm_weights = tm_weights_; }
    static void set_lm_weights(Floats &lm_weights_) { lm_weights = lm_weights_; }
    static void set_rs_weights(Floats &rs_weights_) { rs_weights = rs_weights_; }
    static void set_word_penalty_weight(double word_penalty_weight_) { word_penalty_weight = word_penalty_weight_; }
        
  private:
    
    typedef std::set<Symbol, SymbolSorterMap> SymbolSet;
    typedef std::map<rules::Symbol, SymbolSet> Unmapper;
    
    rules::SimpleDAG parse_to_dag(Lattice&,
                                  Lattice::VertexDescriptor,
                                  Lattice::VertexDescriptor,
                                  Unmapper&);
    rules::SimpleDAG subparse_to_dag(Lattice::EdgeDescriptor,
                                     Lattice&,
                                     std::map<int, int>&,
                                     Unmapper&);
    
    void build_intersector();
    rules::SimpleDAG prune_by_intersector(rules::SimpleDAG&);
    rules::SimpleDAG partial(size_t, std::vector<int>&);
    bool nextksb(std::vector<int>&, int &, int &, int, int);
    
    std::vector<std::pair<Symbol, SListPtr> > word_to_slist(rules::Word&, Unmapper&);
    TransformationPtr word_to_transformation(Symbol&, SListPtr&, rules::Word&, rules::Word&, rules::Word&);
    TransformationSetPtr generate_options( TransformationPtr& );
    
    rules::CompressedDAG src_fsa;
    rules::HuffedWords trg_huf;
    StringVector<> src_sym_map;
    StringVector<> trg_sym_map;    
    
    rules::SimpleDAG intersector;
    
    size_t max_length;          // maximal length of source language rule part (default 7)
    size_t max_nt;              // maximal number of non-terminal symbols (default 4)
    
    unsigned int rule_set_index;
    
    LmContainerPtr lmc;
    
    size_t max_trans_hyper;     // maximal number of transformations per hyper edge (default 20)
    size_t max_hyper_sym;       // maximal number of hyper edges per non-terminal symbol (default 20)
    double eps;              // allowed neglog distance of transformation to best transformation (default -1 = infinity)
                             // (allowed_cost <= best_cost + eps) 
    
    std::set<rules::Symbol> nts;  // set of non-terminal symbols (for easier recognition)

    size_t verbosity;
    size_t cost_length;
    
    static Floats tm_weights;
    static Floats lm_weights;
    static Floats rs_weights;
    static double word_penalty_weight;
};

typedef boost::shared_ptr<RuleSet> RuleSetPtr;

}

}

#endif

#ifndef RULELOADER_HPP__
#define RULELOADER_HPP__

#include "lattice.hpp"
#include "lattice_wrapper.hpp"

#include "Transformation.hpp"
#include "RuleSet.hpp"
#include "LmContainer.hpp"

namespace poleng
{

namespace bonsai
{

class RuleLoader {
  private:
    std::vector<RuleSetPtr> rs;
    
    int max_length;          // maximal length of source language rule part (default 7)
    int max_nt;              // maximal number of non-terminal symbols (default 4)

    LmContainerPtr lmc;

    int max_trans_hyper;     // maximal number of transformations per hyper edge (default 20)
    int max_hyper_sym;       // maximal number of hyper edges per non-terminal symbol (default 20)
    double eps;              // allowed neglog distance of transformation to best transformation (default -1 = infinity)
                             // (allowed_cost <= best_cost + eps) 
    int verbosity;
    
    static Floats tm_weights;
    static Floats lm_weights;
    static Floats rs_weights;
    static double word_penalty_weight;
    
    void merge_edge_transformations(EdgeTransformationsPtr&, EdgeTransformationsPtr&);
    void merge_hyper_edge(HyperEdgePtr &, HyperEdgePtr);
    void fill_empty(Lattice&,
                    Lattice::VertexDescriptor,
                    Lattice::VertexDescriptor,
                    EdgeTransformationsPtr&);

  public:
    RuleLoader(std::string, int, int, LmContainerPtr);
    RuleLoader(int, int, LmContainerPtr);
    
    void add_rule_set(std::string);
    void add_rule_set( RuleSetPtr );
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

};

typedef boost::shared_ptr<RuleLoader> RuleLoaderPtr;

}

}

#endif

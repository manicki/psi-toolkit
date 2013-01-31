#ifndef TRANSFORMATION_H__
#define TRANSFORMATION_H__

#include <vector>
#include <set>
#include <sstream>
#include <boost/shared_ptr.hpp>

#include "TransferTypes.hpp"
#include "LmContainer.hpp"

namespace poleng
{

namespace bonsai
{

class Transformation {
  public:
    Transformation(Symbol, SListPtr, SListPtr, double, Floats, LmContainerPtr);
    Transformation(Symbol, SListPtr, double, Floats, LmContainerPtr);    
    
    bool isSource(Symbol s);
    double get_cost();
    double get_lm_heuristic();
    Floats& get_unweighted();
    SListPtr& sources();
    SListPtr& targets();
    std::string str();

    Symbol& lhs();
    
    void add_alignment(AlignmentPtr &);
    AlignmentPtr& get_alignment();

    static void set_verbosity(int);
    static void set_pedantry(bool);

    static void set_tm_weights(Floats &tm_weights_) { tm_weights = tm_weights_; }
    static void set_lm_weights(Floats &lm_weights_) { lm_weights = lm_weights_; }
    static void set_rs_weights(Floats &rs_weights_) { rs_weights = rs_weights_; }
    static void set_word_penalty_weight(double word_penalty_weight_) { word_penalty_weight = word_penalty_weight_; }
    
  private:
    Symbol srcSymbol;
    SListPtr srcSymbols;
    SListPtr trgSymbols;
    
    double cost;
    Floats unweighted_costs;
    
    LmContainerPtr lmc;
    double lm_heuristic;
    
    static int verbosity;
    static bool pedantry;
    
    static Floats tm_weights;
    static Floats lm_weights;
    static Floats rs_weights;
    static double word_penalty_weight;
    
    AlignmentPtr alignment;
};

typedef boost::shared_ptr<Transformation> TransformationPtr;

struct TransformationSorter {
    bool operator()(const TransformationPtr& lhs, const TransformationPtr& rhs) {
        //std::cerr << "Comparing : " << std::endl;
        //std::cerr << lhs->str() << std::endl;
        //std::cerr << rhs->str() << std::endl;
        
        return lhs->get_cost() + lhs->get_lm_heuristic() < rhs->get_cost() + rhs->get_lm_heuristic()
                or lhs->str() < rhs->str();
    }
};

typedef std::set<TransformationPtr, TransformationSorter> TransformationSet;
typedef boost::shared_ptr<TransformationSet> TransformationSetPtr;  
typedef TransformationSet::iterator TransIt;

class HyperEdge {
  private:
    TransformationSetPtr ts;
    SListPtr nonTerminals;
    
    static Floats tm_weights;
    static Floats lm_weights;
    static Floats rs_weights;
    static double word_penalty_weight;

  public:
    HyperEdge(SListPtr);
    void add(TransformationPtr);
    TransformationSetPtr& get_transformations();
    SListPtr& nts();

    double get_cost();

    static void set_tm_weights(Floats &tm_weights_) { tm_weights = tm_weights_; }
    static void set_lm_weights(Floats &lm_weights_) { lm_weights = lm_weights_; }
    static void set_rs_weights(Floats &rs_weights_) { rs_weights = rs_weights_; }
    static void set_word_penalty_weight(double word_penalty_weight_) { word_penalty_weight = word_penalty_weight_; }

};

typedef boost::shared_ptr<HyperEdge> HyperEdgePtr;

struct HyperEdgeSorter {
    bool operator()(const HyperEdgePtr& lhs, const HyperEdgePtr& rhs) {
        return lhs->get_cost() < rhs->get_cost();
    }
};

typedef std::set<HyperEdgePtr, HyperEdgeSorter> HyperEdgeSet;
typedef boost::shared_ptr<HyperEdgeSet> HyperEdgeSetPtr;

typedef std::map<Symbol, HyperEdgeSetPtr, SymbolSorterMap> EdgeTransformations;
typedef boost::shared_ptr<EdgeTransformations> EdgeTransformationsPtr;


}

}

#endif

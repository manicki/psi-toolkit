#ifndef TRANSLATOR_H__
#define TRANSLATOR_H__

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "lattice.hpp"

#include "TransferTypes.hpp"
#include "Transformation.hpp"
#include "RuleLoader.hpp"

#include "Translation.hpp"
#include "LmContainer.hpp"

namespace poleng
{

namespace bonsai
{

class Translator {
  private:
    
    typedef std::vector<TranslationQueue::iterator> TransCoordinate;
    
    class Candidate {
      private:
	HyperEdgePtr h;
	TranslationPtr t;
	TransIt r0;
	TransCoordinate r;

      public:

	Candidate(TranslationPtr t_, HyperEdgePtr h_, TransIt r0_, TransCoordinate r_) : t(t_), h(h_), r0(r0_), r(r_) {};
	double get_cost() {
	    return t->get_cost();
	}
	
	TranslationPtr& get_translation() { return t; }
	HyperEdgePtr& get_hyper_edge() { return h; }
	TransIt& get_trans_it() { return r0; }
	TransCoordinate& get_trans_coordinate() { return r; }
    };
    
    typedef boost::shared_ptr<Candidate> CandidatePtr;
    
    struct CandidateSorter {
	bool operator()(const CandidatePtr& lhs, const CandidatePtr& rhs) {
	    return lhs->get_cost() < rhs->get_cost();
	}
    };
    
    TranslationQueuePtr& translate_recursive(Symbol&, EdgeTransformationsPtr&);
    TranslationQueuePtr merge(HyperEdgeSetPtr&, EdgeTransformationsPtr&, bool);
    Symbol add_top_symbol(Lattice&,
			  Lattice::VertexDescriptor,
			  Lattice::VertexDescriptor,
			  std::string,
			  EdgeTransformationsPtr&);
    
    RuleLoaderPtr rl;
    LmContainerPtr lm;
    
  public:
    std::map<Symbol, TranslationQueuePtr, SymbolSorterMap> node_translation_memory;
  private:
    
    int k;
    int n;
    
    int verbosity;
    bool mert;
    bool pedantry;
    
    int total_time;
    int sentence_no;
    
    static Floats tm_weights;
    static Floats lm_weights;
    static Floats rs_weights;
    static double word_penalty_weight;
            
  public:
    Translator(RuleLoaderPtr, LmContainerPtr, int);
    
    TranslationQueuePtr translate(Lattice&,
				  Lattice::VertexDescriptor,
				  Lattice::VertexDescriptor,
				  std::string);
    
    void set_sentence_no(int sno) { sentence_no = sno; }
    void set_nbest(int);
    void set_verbosity(int);
    void set_pedantry(bool);
    void set_mert(bool);
    
    double get_total_time() { return total_time; }
    double get_total_sentences() { return sentence_no; }
    
    static void set_tm_weights(Floats &tm_weights_) {
	tm_weights = tm_weights_;
	Translation::set_tm_weights(tm_weights);
	Transformation::set_tm_weights(tm_weights);
	HyperEdge::set_tm_weights(tm_weights);
	RuleLoader::set_tm_weights(tm_weights);
	RuleSet::set_tm_weights(tm_weights);
	LmContainer::set_tm_weights(tm_weights);
    }
    
    static void set_lm_weights(Floats &lm_weights_) {
	lm_weights = lm_weights_;
	Translation::set_lm_weights(lm_weights);
	Transformation::set_lm_weights(lm_weights);
	HyperEdge::set_lm_weights(lm_weights);
	RuleLoader::set_lm_weights(lm_weights);
	RuleSet::set_lm_weights(lm_weights);
	LmContainer::set_lm_weights(lm_weights);
    }

    static void set_rs_weights(Floats &rs_weights_) {
	rs_weights = rs_weights_;
	Translation::set_rs_weights(rs_weights);
	Transformation::set_rs_weights(rs_weights);
	HyperEdge::set_rs_weights(rs_weights);
	RuleLoader::set_rs_weights(rs_weights);
	RuleSet::set_rs_weights(rs_weights);
	LmContainer::set_rs_weights(rs_weights);
    }

    static void set_word_penalty_weight(double word_penalty_weight_) {
	word_penalty_weight = word_penalty_weight_;
	Translation::set_word_penalty_weight(word_penalty_weight_);
	Transformation::set_word_penalty_weight(word_penalty_weight_);
	HyperEdge::set_word_penalty_weight(word_penalty_weight_);
	RuleLoader::set_word_penalty_weight(word_penalty_weight_);
	RuleSet::set_word_penalty_weight(word_penalty_weight_);
	LmContainer::set_word_penalty_weight(word_penalty_weight_);
    }


};

typedef boost::shared_ptr<Translator> TranslatorPtr;

}

}

#endif

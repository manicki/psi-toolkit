#include "Translator.hpp"

//#define DO_NOT_COMBINE_TOP_SYMBOL 1

namespace poleng
{
namespace bonsai
{

/*
    Translator::Factory::Factory(RuleLoaderPtr rl_, LmContainerPtr lm_, int k_)
    {
        this->rl = rl_;
        this->lm = lm_;
        this->k = k_;
    }

    Translator* Translator::Factory::getObject(void)
    {
        Translator* translator = new Translator(this->rl, this->lm, this->k);
        return translator;
    }
*/

Translator::Translator(RuleLoaderPtr rl_, LmContainerPtr lm_, SymInflectorPtr inf_, int k_=20)
    : rl(rl_), lm(lm_), inf(inf_), k(k_), verbosity(0), pedantry(false), n(1), sentence_no(0), total_time(0) {}

Translator::Translator(RuleLoaderPtr rl_, LmContainerPtr lm_, int k_=20)
    : rl(rl_), lm(lm_), k(k_), verbosity(0), pedantry(false), n(1), sentence_no(0), total_time(0) {}

TranslationQueuePtr Translator::translate(ParseGraphPtr& pg) {
    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
    
    node_translation_memory.clear();

    if(verbosity >= 1) {
        std::cerr << "Translating: " << pg->deepest_path()->str() << std::endl;
	
	std::cerr << "Loading rules ..." << std::endl;
    }

    EdgeTransformationsPtr et = rl->get_edge_transformations(pg);
    Symbol top = add_top_symbol(pg, et);
    
    if( inf != false ) {
	inf->set_parse_graph( pg );
    }
    
    if(verbosity >= 1)
	std::cerr << "Starting translation ..." << std::endl;
    
    TranslationQueuePtr translations = translate_recursive(top, et);
    
    if(translations->size() > 0) {
        if(n == 1) {
            TranslationPtr best = *(translations->begin());
            std::cout << best->str() << std::endl;
            
            if(verbosity == 1) {
                std::cerr << "Best: " << best->nice() << std::endl;
            }
            if(verbosity >= 2) {
                std::cerr << best->back_track(0);
		AlignmentPtr a = best->reconstruct_alignment();
		
		std::cerr << std::endl;
		std::cerr << "Inferred word alignment: ";
		for(Alignment::iterator it = a->begin(); it != a->end(); it++) {
		    std::cerr << it->first << "-" << it->second;
		    
		    Alignment::iterator tit = it;
		    tit++;
		    if(tit != a->end()) {
			std::cerr << " ";
		    }
		}
		std::cerr << std::endl;
	    }
        }
        if(n > 1) {
            int c = 0;
            for(TranslationQueue::iterator it = translations->begin(); it != translations->end() and c < n; it++, c++) {
                TranslationPtr act = *it;
                
		if(mert)
		    std::cout << act->mert(sentence_no) << std::endl;
                else 
		    std::cout << act->str() << std::endl;
		    
                if(verbosity == 1 and c == 0) {
                    std::cerr << c+1 << "-best: " << act->nice() << std::endl;
                }
                if(verbosity >= 2 and not mert) {
                    std::cerr << act->back_track(0);
                }
            }
        }
    }
    else {
	std::cerr << "No translation found" << std::endl;
	std::cout << "ERROR" << std::endl;
    }
 
    
    boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta = pt_end - pt_start;
    int d = delta.total_milliseconds();
    total_time += d;
    if(verbosity > 0) {
	std::cerr << "Time: " << d << " ms." << std::endl << std::endl;
    }
    sentence_no++;
    lm->clear_memory();
    
    return translations;
}

TranslationQueuePtr& Translator::translate_recursive(Symbol& s, EdgeTransformationsPtr& et) {
    bool is_top = false;
    if(s.label() == "<TOP>") {
	is_top = true;
    }
    
    if(node_translation_memory.count(s) > 0) {
	return node_translation_memory[s];
    }
    else {
	TranslationQueuePtr node_translations( new TranslationQueue() );
	HyperEdgeSetPtr hyper_edges = (*et)[s];
	node_translations = merge(hyper_edges, et, is_top);
	node_translation_memory[s] = node_translations;
	return node_translation_memory[s];
    }
}

TranslationQueuePtr Translator::merge(HyperEdgeSetPtr& hs, EdgeTransformationsPtr& et, bool top) {
    TranslationQueuePtr k_best_translations( new TranslationQueue() );
    
    std::set<CandidatePtr, CandidateSorter> cands;
   
    for(HyperEdgeSet::iterator h_it = hs->begin(); h_it != hs->end(); h_it++) {
	TransIt r0 = (*h_it)->get_transformations()->begin();
	TransCoordinate r;
	
	TransformationPtr t = *r0;
	TranslationNodes tn;
	for(int i = 0; i < (*h_it)->nts()->size(); i++) {
	    TranslationQueuePtr tq = translate_recursive((*h_it)->nts()->at(i), et);
	    r.push_back(tq->begin());
	    tn[(*h_it)->nts()->at(i)] = *(r[i]);
	}
	TranslationPtr translation( new Translation(t, tn, lm, inf, top) );

// @todo: poprawic to na cos madrzejszego np. kilka symboli <TOP> po maksymalnie n dzieci lub drzewo hierarchiczne
// Lepiej zeby to dzialalo za pomoca jakiejs opcji, a nie ifdefa.
// Niewiadomo jak to wplywa na jakosc tlumaczenia, pewnie nie ma tragedii, ale na pewno sa roznice.
#ifdef DO_NOT_COMBINE_TOP_SYMBOL	
	if(top) {
	    k_best_translations->insert(translation);
	    return k_best_translations;
	}
#endif

	cands.insert(CandidatePtr( new Candidate(translation, *h_it, r0, r) ));
    }
    
    HyperEdgePtr L;
    TransIt r0;
    TransCoordinate r;
    
    while(k_best_translations->size() < k) {
	if(L != false) {
	    for(int i = 0; i <= r.size(); i++) {
		TransIt r0_prim = r0;
		TransCoordinate r_prim = r;
		if(i < r.size()) {
		    r_prim[i]++;
		}
		else if(i == r.size()) {
		    r0_prim++;
		}
		
		if(r0_prim != L->get_transformations()->end()) {
		    TransformationPtr t = *r0_prim;
		    TranslationNodes tn;
		    for(int j = 0; j < L->nts()->size(); j++) {
			TranslationQueuePtr tq = translate_recursive(L->nts()->at(j), et);
			if(r_prim[j] != tq->end())
			    tn[L->nts()->at(j)] = *(r_prim[j]) ;
		    }
		    if(tn.size() == L->nts()->size()) {
			TranslationPtr translation( new Translation(t, tn, lm, inf, top) );
			cands.insert(CandidatePtr( new Candidate(translation, L, r0_prim, r_prim) ));
		    }
		}
	    }
	}
	if(cands.size() > 0) {
	    CandidatePtr c = *cands.begin();
	    cands.erase( cands.begin() );
	    
	    L =  c->get_hyper_edge();
	    r0 = c->get_trans_it();
	    r =  c->get_trans_coordinate();
	    
	    TranslationPtr translation = c->get_translation();
	    if( k_best_translations->count(translation) == 0 )
		k_best_translations->insert( translation );
	    else {
		TranslationPtr t = *(k_best_translations->find(translation));
		if(translation->get_cost() < t->get_cost()) {
		    k_best_translations->insert( translation );
		}
	    }
	}
	else {
	    return k_best_translations;
	}
    }
    
    return k_best_translations;
}

Symbol Translator::add_top_symbol(ParseGraphPtr& pg, EdgeTransformationsPtr& et) {
    // @todo: co roby gdy pg jest pusty?;
    
    SListPtr source = pg->shallow_path();
    
    int start = source->front().start();
    int end =   source->back().end();
    
    Symbol top("<TOP>", Range(start,end), true);
    (*et)[top] = HyperEdgeSetPtr( new HyperEdgeSet() );
    HyperEdgePtr he( new HyperEdge(source) );
    
    SListPtr target( new SList() );
    
    Labels bos;
    Labels eos;
    for(int i=0; i<lm->size(); i++) {
        bos.push_back(Label("<s>"));
        eos.push_back(Label("</s>"));
    }
    
    Symbol BoS(bos, Range(-1,-1), false);
    Symbol EoS(eos, Range(-1,-1), false);
    
    target->push_back(BoS);
    target->insert(target->end(), source->begin(), source->end());
    target->push_back(EoS);
    
    Floats costs;
    he->add( TransformationPtr( new Transformation(top, source, target, 0, costs, lm) ) );
    (*et)[top]->insert(he);
    
    return top;
}

void Translator::set_verbosity(int val) {
    verbosity = val;
    Transformation::set_verbosity(val);
    Translation::set_verbosity(val);
}

void Translator::set_nbest(int val) {
    n = val;
    if(k < n)
        k = n;
}

void Translator::set_pedantry(bool val) {
    pedantry = val;
    Transformation::set_pedantry(val);
    Translation::set_pedantry(val);
}

void Translator::set_mert(bool val) {
    mert = val;
}

Floats Translator::tm_weights;
Floats Translator::lm_weights;
Floats Translator::rs_weights;
double Translator::word_penalty_weight = 0;

}

}


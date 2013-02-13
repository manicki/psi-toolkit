#include "Translation.hpp"
#include <boost/foreach.hpp>

namespace poleng
{

namespace bonsai
{

Translation::Translation(TransformationPtr pt_, TranslationNodes ndtr_, LmContainerPtr lmc_, bool top_) :
  my_id(id++), parent_transformation(pt_), node_translations(ndtr_), lmc(lmc_), top(top_), translation( new SList() ), cost(0), lm_heuristic(0) {
    
    cost = parent_transformation->get_cost();
    
    if(pedantry) {
        unweighted_costs = pt_->get_unweighted();
        unweighted_costs.resize(unweighted_costs.size() + lmc->size(), 0);
    }
    
    for(TranslationNodes::iterator it = node_translations.begin(); it != node_translations.end(); it++) {
        TranslationPtr trans = it->second;
	cost += trans->get_cost();
	
        if(pedantry) {
            Floats add_costs = trans->get_unweighted();
            if(unweighted_costs.size() < add_costs.size())
                unweighted_costs.resize(add_costs.size(), 0);
            for(size_t i=0; i<add_costs.size(); i++)
                unweighted_costs[i] += add_costs[i];
        }
    }
    
    substitute(parent_transformation, node_translations);
    lm_heuristic = lmc->get_front_cost(translation);
}

int Translation::get_id() {
  return my_id;
}

double Translation::get_cost() {
    return cost;
}

Floats& Translation::get_unweighted() {
    return unweighted_costs;
}

std::string Translation::str() {
    if(top) {
        SListPtr final( new SList() );
        for(SList::iterator it = translation->begin(); it != translation->end(); it++) {
            if(it->label() != "<s>" and it->label() != "</s>") {
                final->push_back(*it);
            }
        }
        return final->str();
    }
    else
        return translation->str();
}

std::string Translation::nice() {
    std::stringstream ss;
    ss << "[ " << translation->str();
    if(verbosity > 2) {
	ss << " / " << translation->surface(1);
    }
    ss << " ]\t(";
    if(pedantry) {
        for(size_t i = 0; i < unweighted_costs.size(); i++) {
            ss << unweighted_costs[i];
            if(i < unweighted_costs.size()-1)
                ss << ",";
        }
        ss << " - ";
    }
    ss << get_cost() << ")";
    return ss.str();
}


std::string Translation::mert(int i) {
    std::stringstream ss;
    ss << i << " ||| " << this->str() << " ||| ";
    if(pedantry) {
	for(size_t j = 0; j < unweighted_costs.size(); j++) {
            ss << -1 * unweighted_costs[j];
            if(j < unweighted_costs.size()-1)
                ss << " ";
        }
	ss << " ||| "; 
    }
    ss << -1 * this->get_cost();
    return ss.str();
}

std::string Translation::back_track(int depth = 0) {
    std::stringstream ss;
    std::string indent;
    
    for(size_t i = 0; i < (size_t)depth; i++)
        indent.append("\t");
        
    ss << indent << parent_transformation->str() << std::endl;
    ss << indent << "  " << this->nice() << " - " << lmc->get_plain_cost(translation) << std::endl;
    
    for(SList::iterator it = parent_transformation->targets()->begin(); it != parent_transformation->targets()->end(); it++) {
	if(it->is_nt()) {
	    TranslationPtr tp = node_translations[*it];
    	    ss << tp->back_track(depth+1);
	}
    }
    return ss.str();
}

Lattice::EdgeDescriptor Translation::annotateLattice(Lattice& lattice, std::map<Symbol, Lattice::EdgeDescriptor, SymbolSorterMap2>& symbolEdgeMap, std::map<int, Lattice::EdgeDescriptor>& idEdgeMap)
{
    if(idEdgeMap.count(my_id))
	return idEdgeMap[my_id];


    Lattice::EdgeSequence::Builder partitionBuilder(lattice);
    
    Symbol lhs = parent_transformation->lhs();
    std::string text = this->str();
    Lattice::EdgeDescriptor lhsEdge = symbolEdgeMap[lhs];
	
    partitionBuilder.addEdge(lhsEdge);
    
    std::vector<Lattice::EdgeDescriptor> partition;
    for(SList::iterator it = parent_transformation->targets()->begin(); it != parent_transformation->targets()->end(); it++) {
	if(it->is_nt()) {
	    TranslationPtr tp = node_translations[*it];
	    Lattice::EdgeDescriptor edge = tp->annotateLattice(lattice, symbolEdgeMap, idEdgeMap);
	    	
	    partitionBuilder.addEdge(edge);  
	}
    }
    
    int start = lattice.getEdgeBeginIndex(lhsEdge);
    int end   = lattice.getEdgeEndIndex(lhsEdge);
    
    AnnotationItem ai("TRANS[" + lhs.label() +"]", StringFrag(text));
    std::vector<std::string> tagVector;
    if(top)
	tagVector.push_back("trans");
    else
	tagVector.push_back("transfrag");
    tagVector.push_back("bonsai");
    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollection(tagVector);
    idEdgeMap[my_id] = lattice.addEdge(start, end, ai, tags, partitionBuilder.build(), this->get_cost());
    return idEdgeMap[my_id];
}

std::string Translation::dump_to_perl() {
    std::stringstream ss;
        
    ss << "{ ";
    ss << "transformation => q{" << parent_transformation->str() << "}, ";
    ss << "translation => q{" << this->str() << "}, ";
    
    ss << "sub => [ ";
    bool first = true;
    for(SList::iterator it = parent_transformation->targets()->begin(); it != parent_transformation->targets()->end(); it++) {
	if(not first) {
	    ss << ", ";
	}
	if(it->is_nt()) {
	    TranslationPtr tp = node_translations[*it];
    	    ss << tp->dump_to_perl();
	    first = false;
	}
    }
    ss << " ]";
    
    ss << " } ";
    return ss.str();
}

AlignmentPtr Translation::reconstruct_alignment() {
    AlignmentPtr a(new Alignment());
    
    int tpos = 0;
    reconstruct_alignment_rec(a, tpos);
    
    return a;
}

void Translation::reconstruct_alignment_rec(AlignmentPtr &a, int &tpos) {
    // do poprawy
    
    int i = 1;
    BOOST_FOREACH( Symbol s, *parent_transformation->targets() ) {
	if( s.is_nt() ) {
	    TranslationPtr tp = node_translations[s];
    	    tp->reconstruct_alignment_rec(a, tpos);
	}
	else {
	    BOOST_FOREACH( AlignmentPoint rel_ap, *parent_transformation->get_alignment() ) {
	        if( rel_ap.second == i ) {
		    int first  = parent_transformation->sources()->at(rel_ap.first-1).end();
		    int second = tpos;
		    a->insert( AlignmentPoint( first, second ) );
		}
	    }
	    tpos++;
	}
	i++;
    }
}

double Translation::get_lm_heuristic() {
    //return 0;
    return lm_heuristic;
}

void Translation::substitute(TransformationPtr &t, TranslationNodes &tn) {
    
    if(tn.size() == 0) {
	translation = t->targets();
	    	
	Floats lm_costs;
	double lm_cost = lmc->get_cost(translation, lm_costs);
	cost += lm_cost;
	
	if(pedantry) {
	    for(size_t i = 0; i<lm_costs.size(); i++) {
		size_t j = unweighted_costs.size() - lm_costs.size() + i;
		unweighted_costs[j] += lm_costs[i];
	    }
	}
    }
    else {
        LinkPoints lp;
  
        int c = 0;
        int i = 0;
	int j = 1;
        
        SListPtr ngram( new SList() );
        for(SList::iterator it = t->targets()->begin(); it != t->targets()->end(); it++) {
            if(it->is_nt()) {
                if(ngram->size() > 0) {
                    Floats lm_costs;
                    double lm_cost = lmc->get_cost(translation, lm_costs);
                    cost += lm_cost;
        
                    //std::cerr << "Ngram: " << ngram->str() << " - " << lm_cost << std::endl;
        
                    if(pedantry) {
                        for(size_t k = 0; k<lm_costs.size(); k++) {
                            size_t l = unweighted_costs.size() - lm_costs.size() + k;
                            unweighted_costs[l] += lm_costs[k];
                        }
                    }  
                    ngram.reset( new SList() );
                }
              
                if(it > t->targets()->begin()) {
                    lp.insert(Link(i-1,i));
                }
                
                int l = tn[*it]->translation->size();
                i += l;
                translation->insert(translation->end(), tn[*it]->translation->begin(), tn[*it]->translation->end());
                c++;
                
                if(it < (t->targets()->end()-1)) {
                    lp.insert(Link(i-1,i));
                }
            }
            else {
		translation->push_back( *it );
		ngram->push_back( *it );
                i++;
            }
	    j++;
        }
        
        if(ngram->size() > 0) {
            Floats lm_costs;
            double lm_cost = lmc->get_cost(ngram, lm_costs);
            cost += lm_cost;
            
            //std::cerr << "Ngram: " << ngram->str() << " - " << lmc->get_plain_cost(ngram) << std::endl;
            
            if(pedantry) {
                for(size_t k = 0; k<lm_costs.size(); k++) {
                    size_t l = unweighted_costs.size() - lm_costs.size() + k;
                    unweighted_costs[l] += lm_costs[k];
                }
            }  
            ngram.reset( new SList() );
        }
        
        Floats lm_costs;
        double lmcost = lmc->get_cost(translation, lm_costs, lp);
        cost += lmcost;
        
        if(pedantry) {
            for(size_t k = 0; k<lm_costs.size(); k++) {
                size_t l = unweighted_costs.size() - lm_costs.size() + k;
                unweighted_costs[l] += lm_costs[k];
            }
        }
    }
    
}

int Translation::id = 0;
int Translation::verbosity = 0;
bool Translation::pedantry = false;

void Translation::set_verbosity(int val) {
    verbosity = val;
}

void Translation::set_pedantry(bool val) {
    pedantry = val;
}

Floats Translation::tm_weights;
Floats Translation::lm_weights;
Floats Translation::rs_weights;
double Translation::word_penalty_weight = 0;

}

}


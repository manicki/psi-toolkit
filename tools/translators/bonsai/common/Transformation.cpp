#include <Transformation.hpp>

using namespace poleng::bonsai;

Transformation::Transformation(Symbol srcSymbol_, SListPtr srcSymbols_, SListPtr trgSymbols_, double cost_, Floats ec_, LmContainerPtr lmc_) :
    srcSymbol(srcSymbol_), srcSymbols(srcSymbols_), trgSymbols(trgSymbols_), cost(cost_), unweighted_costs(ec_), lmc(lmc_), lm_heuristic(0), alignment( new Alignment() ) {
        //lm_heuristic = lmc->get_terminal_cost(trgSymbols);
    }
    
Transformation::Transformation(Symbol srcSymbol_, SListPtr trgSymbols_, double cost_, Floats ec_, LmContainerPtr lmc_) :
    srcSymbol(srcSymbol_), srcSymbols(SListPtr( new SList() )), trgSymbols(trgSymbols_), cost(cost_), unweighted_costs(ec_), lmc(lmc_), lm_heuristic(0), alignment( new Alignment() ) {
        //lm_heuristic = lmc->get_terminal_cost(trgSymbols);
    }
    
bool Transformation::isSource(Symbol s) {
    return s.str() == srcSymbol.str();
}

double Transformation::get_cost() {
    return cost;
}

Symbol& Transformation::lhs() {
    return srcSymbol;
}

double Transformation::get_lm_heuristic() {
    return lm_heuristic;
}

Floats& Transformation::get_unweighted() {
    return unweighted_costs;
}

SListPtr& Transformation::sources() {
    return srcSymbols;  
}

SListPtr& Transformation::targets() {
    return trgSymbols;  
}

std::string Transformation::str() {
    std::stringstream ss;
    ss << srcSymbol.str() << " --> " << srcSymbols->str() << " :: " << trgSymbols->str();
    
    {
	ss << "\t(";
	for(Alignment::iterator ait = alignment->begin(); ait != alignment->end(); ait++) {
	    if(ait != alignment->begin())
		ss << " ";
	    ss << ait->first << "-" << ait->second;
	}
	ss << ")";
    }
    
    if(pedantry) {
	ss << "\t(" ;
	for(int i = 0; i < unweighted_costs.size(); i++) {
	    ss << unweighted_costs[i];
	    if(i < unweighted_costs.size()-1)
		ss << ",";
	}
	ss << " - ";
	ss << cost  << ")";
    }
    
    return ss.str();
}

void Transformation::add_alignment(AlignmentPtr &a) {
    alignment = a;
}

AlignmentPtr& Transformation::get_alignment() {
    return alignment;
}


int Transformation::verbosity = 0;
void Transformation::set_verbosity(int val) {
    verbosity = val;
}

bool Transformation::pedantry = false;
void Transformation::set_pedantry(bool val) {
    pedantry = val;
}

Floats Transformation::tm_weights;
Floats Transformation::lm_weights;
Floats Transformation::rs_weights;
double Transformation::word_penalty_weight = 0;


/*********************** HyperEdge ************************/

HyperEdge::HyperEdge(SListPtr src) : ts( new TransformationSet() ), nonTerminals( new SList() ) {
    for(SList::iterator it = src->begin(); it != src->end(); it++) {
	if(it->is_nt()) {
	    nonTerminals->push_back(*it);
	}
    }
}

void HyperEdge::add(TransformationPtr t) {
    ts->insert(t);
}

TransformationSetPtr& HyperEdge::get_transformations() {
    return ts;
}

SListPtr& HyperEdge::nts() {
    return nonTerminals;
}

double HyperEdge::get_cost() {
    if(ts->size() > 0) {
	return (*(ts->begin()))->get_cost();
    }
    else {
	return 0;
    }
}

Floats HyperEdge::tm_weights;
Floats HyperEdge::lm_weights;
Floats HyperEdge::rs_weights;
double HyperEdge::word_penalty_weight = 0;



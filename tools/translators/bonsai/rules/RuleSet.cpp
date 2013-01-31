#include "RuleSet.hpp"
#include <cstdio>

namespace poleng
{

namespace bonsai
{

RuleSet::RuleSet(std::string path, int max_length_, int max_nt_, int rule_set_index_, LmContainerPtr lmc_) 
 : max_length(max_length_), max_nt(max_nt_), rule_set_index(rule_set_index_),
   lmc(lmc_), max_trans_hyper(20), max_hyper_sym(20), eps(-1), verbosity(0),
   cost_length(5) {
   namespace po = boost::program_options;
   po::options_description desc("rule options");
   desc.add_options()
       ("source.target.map", po::value<std::string>()->default_value("plfr.map"), "")
       ("source.rules.index", po::value<std::string>()->default_value("plfr.src.idx"), "")
       ("source.rules.symbols", po::value<std::string>()->default_value("plfr.src.symbols"), "")
       ("source.rules.mode", po::value<int>()->default_value(0), "")
       ("target.rules.index", po::value<std::string>()->default_value("plfr.trg.idx"), "")
       ("target.rules.symbols", po::value<std::string>()->default_value("plfr.trg.symbols"), "")
       ("target.rules.costs", po::value<int>()->default_value(5), "")
   //    ("target.rules.mode", po::value<int>()->default_value(0), "")
   ;

   boost::program_options::variables_map vm;
   std::ifstream config((path + "/transfer.ini").c_str());
   boost::program_options::store(boost::program_options::parse_config_file(config, desc), vm);
   boost::program_options::notify(vm);
       
   if(vm.count("source.target.map") == 0) {
       std::cerr << "Error: No rule map file given." << std::endl;        
       exit(1);
   }
   std::string mapfile = path + "/" + vm["source.target.map"].as<std::string>();
   
   if(vm.count("source.rules.index") == 0) {
       std::cerr << "Error: No source rule index file given." << std::endl;        
       exit(1);
   }
   std::string sindex = path + "/" + vm["source.rules.index"].as<std::string>();
   
   
   if(vm.count("source.rules.symbols") == 0) {
       std::cerr << "Error: No source rule symbols file given." << std::endl;        
       exit(1);
   }
   std::string ssymbols = path + "/" + vm["source.rules.symbols"].as<std::string>();
   
   if(vm.count("target.rules.index") == 0) {
       std::cerr << "Error: No target rule index file given." << std::endl;        
       exit(1);
   }
   std::string tindex = path + "/" + vm["target.rules.index"].as<std::string>();
           
   if(vm.count("target.rules.symbols") == 0) {
       std::cerr << "Error: No target rule symbols file given." << std::endl;        
       exit(1);
   }
   std::string tsymbols = path + "/" + vm["target.rules.symbols"].as<std::string>();
   
   cost_length = vm["target.rules.costs"].as<int>();
   if(cost_length != tm_weights.size()) {
      std::cerr << "Warning. Translation model weight vector and rule parameter weight vector have different lengths.";
   }
   
   int smode = vm["source.rules.mode"].as<int>();
   //int tmode = vm["target.rules.mode"].as<int>();
    
   src_fsa.open(sindex, smode);
   
    if(verbosity > 1)
      std::cerr << "Loading symbols" << std::endl;
   
   src_sym_map.load(ssymbols);
   trg_sym_map.load(tsymbols);
   
    if(verbosity > 1)
      std::cerr << "Loading huffed" << std::endl;
   
   trg_huf.open(tindex);
   
   build_intersector();
}

EdgeTransformationsPtr RuleSet::get_edge_transformations(Lattice &lattice,
							 Lattice::VertexDescriptor start,
							 Lattice::VertexDescriptor end) {
    Unmapper src_sym_unmap;
    
    boost::posix_time::ptime pt_start1 = boost::posix_time::microsec_clock::local_time();
    
    rules::SimpleDAG src_lang_dag = parse_to_dag(lattice, start, end, src_sym_unmap);
 
    boost::posix_time::ptime pt_start2 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta1 = pt_start2 - pt_start1;

    if(verbosity > 0)
	std::cerr << "DAG created in " << delta1.total_milliseconds() << " ms." << std::endl;

    rules::WordList src_lang_wordlist = src_fsa.multihash(src_lang_dag);
   
    boost::posix_time::ptime pt_start3 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta2 = pt_start3 - pt_start2;

    if(verbosity > 0)
	std::cerr << "DAG intersected with rules in " << delta2.total_milliseconds() << " ms." << std::endl;
    
    if(verbosity > 0) 
	std::cerr << "Rules found: " << src_lang_wordlist.size() << std::endl;
    
    EdgeTransformationsPtr et( new EdgeTransformations() );    
    typedef std::vector<std::pair<Symbol, SListPtr> > SymbolSListPairs;
    
    rules::WordIndexSorter wi_sorter;
    std::sort(src_lang_wordlist.begin(), src_lang_wordlist.end(), wi_sorter);
    for(rules::WordList::iterator it = src_lang_wordlist.begin(); it != src_lang_wordlist.end(); it++) {
	
	rules::Word src_word = it->get<0>();
	int pos = it->get<1>()-1;        
	
	if(pos >= 0) {
	    rules::WordTriples trg_wordtriples = trg_huf.get_wordtriples(pos);
	    SymbolSListPairs sls = word_to_slist(src_word, src_sym_unmap);
	    
	    for(SymbolSListPairs::iterator slit = sls.begin(); slit != sls.end(); slit++) {    
		if(et->count(slit->first) == 0)
		    (*et)[slit->first] = HyperEdgeSetPtr( new HyperEdgeSet() );
		
		HyperEdgePtr he( new HyperEdge(slit->second) );
		
		for(rules::WordTriples::iterator wit = trg_wordtriples.begin(); wit != trg_wordtriples.end(); wit++) {
	   		    
		    rules::Word trg_word  = wit->get<0>();
		    rules::Word trg_probs = wit->get<1>();
		    rules::Word trg_align = wit->get<2>();
		      
		    TransformationPtr tp = word_to_transformation(slit->first, slit->second, trg_word, trg_probs, trg_align);
		    
		    if(verbosity > 1)
			std::cerr << tp->str() << std::endl;
		    
		    TransformationSetPtr tsp( new TransformationSet() );
		    tsp->insert( tp );
		    
		    BOOST_FOREACH( TransformationPtr t, *tsp ) {
			if(he->get_transformations()->size() < max_trans_hyper) {
			    if(eps > 0 and he->get_transformations()->size() > 0) {
				TransformationPtr best = *(he->get_transformations()->begin());
				if(t->get_cost() <= best->get_cost() + eps) {
				    he->add(t);
				}
			    }
			    else {
				he->add(t);
			    }
			}
		    }
		}
		
		(*et)[slit->first]->insert(he);
		
		while((*et)[slit->first]->size() > max_hyper_sym) {
		    HyperEdgeSet::iterator it = (*et)[slit->first]->end();
		    it--;
		    (*et)[slit->first]->erase(it);
		}
	    }
	
	}
	else {
	    std::cerr << it->get<1>()-1 << std::endl;
	}
        
    }
      
    boost::posix_time::ptime pt_all_end = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta_all = pt_all_end - pt_start1;
    
    if(verbosity > 0) {
	std::cerr << "All transformations loaded in " << delta_all.total_milliseconds() << " ms." << std::endl;
    }

    return et;
}

TransformationPtr RuleSet::word_to_transformation(Symbol &lhs, SListPtr &srcSymbols, rules::Word &word, rules::Word &probs, rules::Word &align) {
   SListPtr trgSymbols ( new SList() );
   Floats costs;
   
   int non_terminals = 0;
   for(size_t i = 0; i < word.size(); i++) {
	std::string trgSymbol = trg_sym_map[word[i]];
	int srcIndex;
	if(std::sscanf(trgSymbol.c_str(), "<X>[%d]", &srcIndex)) {
	    int i = srcSymbols->nt_index( srcIndex );
	    if(i != -1) {
        	trgSymbols->push_back(srcSymbols->at(i));
        	non_terminals++;
	    }
	    else {
        	trgSymbols->push_back(Symbol(trgSymbol, Range(-1,-1)));
	    }
	}
        else {
            trgSymbols->push_back(Symbol(trgSymbol, Range(-1,-1)));
        }
    }
     
   double cost = 0;
   size_t i, j;
   for(i = 0, j = 0; j < tm_weights.size(); i++, j++) { // only tm_weight.size() costs are used 
       if(i < probs.size()) {                                                                                                 
            try {                                                                                                             
                costs.push_back(boost::lexical_cast<float>( trg_sym_map[probs[i]] ));            
            }                                                                                                                 
            catch(...) {                                                                                                      
        	costs.push_back(0);                                                                                           
            }                                                                                                                 
	}                                                                                                                   
       else
	  costs.push_back(0);
	  
       cost += (tm_weights[j] * costs[j]);
   }
   costs.push_back(trgSymbols->size() - non_terminals); // transformation length in terminal symbols
   cost += ( word_penalty_weight * costs.back() );
   
   Floats rs_vector;
   rs_vector.resize(rs_weights.size(), 0);
   rs_vector[rule_set_index] = 1;
   
   for(size_t i = 0; i < rs_vector.size(); i++) {
      costs.push_back(rs_vector[i]);
      cost += rs_weights[i] * rs_vector[i];
   }
   
   AlignmentPtr a( new Alignment );
   for(size_t i = 0; i < align.size(); i+=2) {
	a->insert(AlignmentPoint(align[i], align[i+1]));
   }
   
   TransformationPtr t( new Transformation(lhs, srcSymbols, trgSymbols, cost, costs, lmc) );
   t->add_alignment(a);
   
   return t;
}

std::vector<std::pair<Symbol, SListPtr> > RuleSet::word_to_slist(rules::Word &w, Unmapper &unmap) {    
    std::vector<std::pair<Symbol, SListPtr> > sl;
    
    for(SymbolSet::iterator it = unmap[w[0]].begin(); it != unmap[w[0]].end(); it++) {
	std::stack<std::pair<int, Symbol> > S;
	
	Symbol lhs = *it;
	
        int start = 1;
        for(SymbolSet::iterator it = unmap[w[start]].begin(); it != unmap[w[start]].end(); it++) {
            Symbol t = *it;
	    if(t.start() == lhs.start() and t.end() <= lhs.end() and t != lhs)
		S.push(std::make_pair(start, t));
	}
	
	SListPtr path( new SList() );
	 
	while(!S.empty()) {
        std::pair<int, Symbol> s = S.top();
            S.pop();
	    
            int act = s.first;
            Symbol sym = s.second;
	    
	    while(path->size() > 0 and path->back().start() >= sym.start()) {
		path->pop_back();
	    }
            
            path->push_back(sym);
            
            if(path->size() == w.size()-1 and act == (int)w.size()-1 and sym.end() == lhs.end()) {
		sl.push_back(std::make_pair(lhs, path));
		break;
            }
            
	    if( path->size() <= w.size() and act < (int)w.size()-1) {
		act++;
                for(SymbolSet::iterator it = unmap[w[act]].begin(); it != unmap[w[act]].end(); it++) {
		    Symbol t = *it;
                    if(sym.end() == t.start() and t.end() <= lhs.end())
			S.push(std::make_pair(act, t));
		}
	    }
	}
    }
    return sl;
}

rules::SimpleDAG RuleSet::parse_to_dag(Lattice &lattice,
				       Lattice::VertexDescriptor start,
				       Lattice::VertexDescriptor end,
				       Unmapper &unmap) {
    rules::SimpleDAG ndag;
    
    Lattice::EdgeSequence treeSymbols = getTreeSymbols(lattice, start, end);
    std::map<int, int> charTokenMap = getCharWordTokenMap(lattice, start, end);
      
    Lattice::EdgeSequence::Iterator treeSymbolsIt(lattice, treeSymbols);  
    while(treeSymbolsIt.hasNext()) {
	Lattice::EdgeDescriptor symbol = treeSymbolsIt.next();
	if(isNonTerminal(symbol, lattice)) {
	    rules::SimpleDAG tdag = subparse_to_dag(symbol, lattice, charTokenMap, unmap);
	    ndag.nd_union(tdag);
	}
    }
    ndag = prune_by_intersector(ndag);
    return ndag;
}

rules::SimpleDAG RuleSet::subparse_to_dag(Lattice::EdgeDescriptor lhs,
					  Lattice &lattice,
					  std::map<int, int>& charTokenMap,
					  Unmapper &unmap)
{    
    rules::SimpleDAG ndag;
    
    std::map<int, rules::State> mapper;
    
    std::string lhsCategory = lattice.getAnnotationCategory(lhs);
    std::string lhsLabel = "<" + lhsCategory + ">";
  
    rules::Symbol symbol = src_sym_map.find(lhsLabel) + 1;    
    
    int lhsStart = charTokenMap[lattice.getEdgeBeginIndex(lhs)];
    int lhsEnd   = charTokenMap[lattice.getEdgeEndIndex(lhs)];
    
    if(unmap.count(symbol) == 0) {
        SymbolSet t;
        unmap[symbol] = t;
    }
    unmap[symbol].insert(Symbol(lhsLabel, Range(lhsStart, lhsEnd), true));
    nts.insert(symbol);
    
    rules::State q0 = ndag.new_state(true);
    rules::State q1 = ndag.new_state(false);
    rules::State qEnd = ndag.new_state(false);
    ndag.set_end_state(qEnd);
      
    ndag.new_arc(q0, q1, symbol, 0);
    
    mapper[lhsStart] = q1;
    mapper[lhsEnd] = qEnd;
    
    Lattice::EdgeSequence subTree = getSubTreeSymbols(lhs, lattice);
    Lattice::EdgeSequence::Iterator subTreeIt(lattice, subTree);
    while(subTreeIt.hasNext()) {
	Lattice::EdgeDescriptor edge = subTreeIt.next();    
	bool isNT = isNonTerminal(edge, lattice);
	
	std::string edgeLabel;
	if(isNT)
	    edgeLabel = "<" + lattice.getAnnotationCategory(edge) + ">";
	else
	    edgeLabel = lattice.getEdgeText(edge);
      
	rules::Symbol symbol = src_sym_map.find(edgeLabel) + 1;    
	
	int edgeStart = charTokenMap[lattice.getEdgeBeginIndex(edge)];
	int edgeEnd   = charTokenMap[lattice.getEdgeEndIndex(edge)];
	
	if(unmap.count(symbol) == 0) {
	    SymbolSet t;
	    unmap[symbol] = t;
	}
	unmap[symbol].insert(Symbol(edgeLabel, Range(edgeStart, edgeEnd), isNT));
	
	if(isNT)
	    nts.insert(symbol);
	
	if(!mapper.count(edgeStart))
	    mapper[edgeStart] = ndag.new_state(false);
    
	if(!mapper.count(edgeEnd))
	    mapper[edgeEnd] = ndag.new_state(false);
	  
	ndag.new_arc(mapper[edgeStart], mapper[edgeEnd], symbol, 0);
    }
    
    return ndag;
}

rules::SimpleDAG RuleSet::prune_by_intersector(rules::SimpleDAG &d) {
    rules::SimpleDAG ndag;

    d.minimize();
    typedef std::pair<rules::State,rules::State> SPair;
    
    std::vector<SPair> queue;
    std::map<SPair, rules::State> mapper;
    
    SPair start(0,0);
    mapper[start] = ndag.new_state(true);
    
    queue.push_back(start);
    
    while(!queue.empty()) {
	SPair ns = queue.back();
	queue.pop_back();
	
	if(intersector.is_end_state(ns.first) and d.is_end_state(ns.second)) {
	    ndag.set_end_state(mapper[ns]);
	}
	
	rules::Range r1 = intersector.row(ns.first);
	rules::Range r2 = d.row(ns.second);
	
	std::map<int, rules::State> intmap;
	for(rules::ArcIt intit = r1.first; intit != r1.second; intit++) {
	    intmap[intit->a] = intit->q; 	
	} 
	
	for(rules::ArcIt ait = r2.first; ait != r2.second; ait++) {
	    if(intmap.count(nts.count(ait->a)) == 1) {
		SPair t(intmap[nts.count(ait->a)], ait->q);
		if(mapper.count(t) == 0) {
		    queue.push_back(t);
		    mapper[t] = ndag.new_state(false);
		}
		ndag.new_arc(mapper[ns], mapper[t], ait->a, 0);
	    }
	}
    }
    ndag.minimize();
    return ndag;	
}

void RuleSet::build_intersector() {

    int n = max_length; int k = max_nt;
    int m, h; std::vector<int> a; 
    
    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
    
    rules::SimpleDAG ndag = partial(n, a);
    intersector.nd_union(ndag);
    
    while(nextksb(a,h,m,k,n)) {
	rules::SimpleDAG kndag = partial(n, a);
	intersector.nd_union(kndag);
    }
    
    intersector.minimize();
    
    boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta = pt_end - pt_start;
    if(verbosity > 0)
	std::cerr << "Intersector created in " << delta.total_milliseconds() << " ms." << std::endl;
	       
}

rules::SimpleDAG RuleSet::partial(size_t n, std::vector<int> &a) {
    rules::SimpleDAG ndag;
    rules::State q00 = ndag.new_state(true);
    rules::State q0 = ndag.new_state(false);
    ndag.new_arc(q00,q0,1,0);
    
    rules::State q = q0;
    for(size_t i = 0, j = 0; i < n; i++) {
	rules::State p = ndag.new_state(false);
	ndag.set_end_state(p);
	
	ndag.new_arc(q, p, 0, 0);
	if(j < a.size() and (size_t)a[j] == i+1) {
	    ndag.new_arc(q, p, 1, 0);
	    j++;
	}
	q = p;
    }
    
    return ndag;
}

bool RuleSet::nextksb(std::vector<int> &a, int &h, int &m, int k, int n) {
    
    if(k == 0)
	return false;
	 
    bool first = false;
    
    if(a.size() != (size_t)k) {
	a.clear();
	a.resize(k,0);
	first = true;
    }
    
    if(a[0] == n-k+1) return false;
    
    if(first) {
	m = 0;
	h = k;
    }
    else {
	if(m < n-h)
	    h = 0;		    
    
	h++;
	m = a[k-h];
    }
    
    for(int j = 1; j <= h; j++)
	a[k+j-h-1] = m+j;

    return true;
}
    
void RuleSet::set_verbosity(int val) {
    verbosity = val;
}

void RuleSet::set_max_transformations_per_hyperedge(int val) {
    max_trans_hyper = val;
}

void RuleSet::set_max_hyperedges_per_nonterminal(int val) {
    max_hyper_sym = val;
}

void RuleSet::set_max_transformation_factor(double val) {
    if(val > 0)
        eps = -log(val);
}

Floats RuleSet::tm_weights;
Floats RuleSet::lm_weights;
Floats RuleSet::rs_weights;
double RuleSet::word_penalty_weight = 0;

}

}


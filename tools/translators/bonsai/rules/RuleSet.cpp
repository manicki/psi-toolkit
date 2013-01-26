#include "RuleSet.hpp"

namespace poleng
{

namespace bonsai
{

//RuleSet::RuleSet(std::string path, int max_length_, int max_nt_, int rule_set_index_, LmContainerPtr lmc_/*SymInflectorPtr inf_*/) 
// : max_length(max_length_), max_nt(max_nt_), rule_set_index(rule_set_index_), lmc(lmc_), /*inf(inf_)*/ max_trans_hyper(20), max_hyper_sym(20), eps(-1), verbosity(0), cost_length(5) {
//   namespace po = boost::program_options;
//   po::options_description desc("rule options");
//   desc.add_options()
//       ("source.target.map", po::value<std::string>()->default_value("plfr.map"), "")
//       ("source.rules.index", po::value<std::string>()->default_value("plfr.src.idx"), "")
//       ("source.rules.symbols", po::value<std::string>()->default_value("plfr.src.symbols"), "")
//       ("source.rules.mode", po::value<int>()->default_value(0), "")
//       ("target.rules.index", po::value<std::string>()->default_value("plfr.trg.idx"), "")
//       ("target.rules.symbols", po::value<std::string>()->default_value("plfr.trg.symbols"), "")
//       ("target.rules.costs", po::value<int>()->default_value(5), "")
//       ("target.rules.mode", po::value<int>()->default_value(0), "")
//   ;
//
//   boost::program_options::variables_map vm;
//   std::ifstream config((path + "/transfer.ini").c_str());
//   boost::program_options::store(boost::program_options::parse_config_file(config, desc), vm);
//   boost::program_options::notify(vm);
//       
//   if(vm.count("source.target.map") == 0) {
//       std::cerr << "Error: No rule map file given." << std::endl;        
//       exit(1);
//   }
//   std::string mapfile = path + "/" + vm["source.target.map"].as<std::string>();
//   
//   if(vm.count("source.rules.index") == 0) {
//       std::cerr << "Error: No source rule index file given." << std::endl;        
//       exit(1);
//   }
//   std::string sindex = path + "/" + vm["source.rules.index"].as<std::string>();
//   
//   
//   if(vm.count("source.rules.symbols") == 0) {
//       std::cerr << "Error: No source rule symbols file given." << std::endl;        
//       exit(1);
//   }
//   std::string ssymbols = path + "/" + vm["source.rules.symbols"].as<std::string>();
//   
//   if(vm.count("target.rules.index") == 0) {
//       std::cerr << "Error: No target rule index file given." << std::endl;        
//       exit(1);
//   }
//   std::string tindex = path + "/" + vm["target.rules.index"].as<std::string>();
//           
//   if(vm.count("target.rules.symbols") == 0) {
//       std::cerr << "Error: No target rule symbols file given." << std::endl;        
//       exit(1);
//   }
//   std::string tsymbols = path + "/" + vm["target.rules.symbols"].as<std::string>();
//   
//   cost_length = vm["target.rules.costs"].as<int>();
//   if(cost_length != tm_weights.size()) {
//      std::cerr << "Warning. Translation model weight vector and rule parameter weight vector have different lengths.";
//   }
//   
//   int smode = vm["source.rules.mode"].as<int>();
//   int tmode = vm["target.rules.mode"].as<int>();
//   
//   src_trg_map.open(mapfile);    
//    
//   src_fsa.open(sindex, smode);
//   src_sym_map.open(ssymbols);
//   
//   trg_huf.open(tindex);
//   trg_sym_map.open(tsymbols);
//     
//   build_intersector();
//}

RuleSet::RuleSet(std::string path, int max_length_, int max_nt_, int rule_set_index_, LmContainerPtr lmc_) 
 : max_length(max_length_), max_nt(max_nt_), rule_set_index(rule_set_index_), lmc(lmc_), max_trans_hyper(20), max_hyper_sym(20), eps(-1), verbosity(0), cost_length(5) {
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
       ("target.rules.mode", po::value<int>()->default_value(0), "")
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
   int tmode = vm["target.rules.mode"].as<int>();
   
   std::ifstream mapstream(std::string(mapfile + ".txt").c_str());
   src_trg_map.read_text_data(mapstream);    
    
   src_fsa.open(sindex, smode);
   src_sym_map.open(ssymbols);
   
   trg_huf.open(tindex);
   trg_sym_map.open(tsymbols);
   
   
   build_intersector();
}

EdgeTransformationsPtr RuleSet::get_edge_transformations(ParseGraphPtr &pg) {
    Unmapper src_sym_unmap;
    
    boost::posix_time::ptime pt_start1 = boost::posix_time::microsec_clock::local_time();
    
    rules::SimpleDAG src_lang_dag = parse_to_dag(pg, src_sym_unmap);
//    if( inf != false )
//	inf->set_parse_graph( pg );
 
    boost::posix_time::ptime pt_start2 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta1 = pt_start2 - pt_start1;

    if(verbosity > 0)
	std::cerr << "DAG created in " << delta1.total_milliseconds() << " ms." << std::endl;

    rules::WordList src_lang_wordlist = src_fsa.multihash(src_lang_dag);
    
    boost::posix_time::ptime pt_start3 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta2 = pt_start3 - pt_start2;

    if(verbosity > 0)
	std::cerr << "DAG intersected with rules in " << delta2.total_milliseconds() << " ms." << std::endl;

    EdgeTransformationsPtr et( new EdgeTransformations );    
    typedef std::vector<std::pair<Symbol, SListPtr> > SymbolSListPairs;
    
    rules::WordIndexSorter wi_sorter;
    std::sort(src_lang_wordlist.begin(), src_lang_wordlist.end(), wi_sorter);
    for(rules::WordList::iterator it = src_lang_wordlist.begin(); it != src_lang_wordlist.end(); it++) {
	rules::Word src_word = it->get<0>();
	int pos = it->get<1>();
	
	int trg_bytepos1 = src_trg_map.at(it->get<1>()-1);
	int trg_bytepos2 = src_trg_map.at(it->get<1>());

	int trg_length   = trg_bytepos2-trg_bytepos1;
	
	std::cerr << pos << " : ";
	BOOST_FOREACH ( int i, src_word ) {
	  std::cerr << i << " (" << src_sym_map.get_label_by_number(i) << ") ";
	}
	std::cerr << std::endl;
	
        std::cerr << trg_bytepos1 << " - " << trg_bytepos2 << std::endl;
        
	if(trg_bytepos1 >= 0 and trg_length >= 0) {
	    rules::WordTriples trg_wordtriples = trg_huf.get_wordtriples(trg_bytepos1, trg_length);
	    
	    SymbolSListPairs sls = word_to_slist(src_word, src_sym_unmap);
	    for(SymbolSListPairs::iterator slit = sls.begin(); slit != sls.end(); slit++) {    
		if(et->count(slit->first) == 0)
		    (*et)[slit->first] = HyperEdgeSetPtr( new HyperEdgeSet() );
		
		HyperEdgePtr he( new HyperEdge(slit->second) );
		
		for(rules::WordTriples::iterator wit = trg_wordtriples.begin(); wit != trg_wordtriples.end(); wit++) {
	   		    
		    rules::Word trg_word  = wit->get<0>();
		    rules::Word trg_probs = wit->get<1>();
		    rules::Word trg_align = wit->get<2>();
		      
		    // @todo: finish function word_to_transformation
		    TransformationPtr tp = word_to_transformation(slit->first, slit->second, trg_word, trg_probs, trg_align);
		    // Generate translation options here !
		    
		    TransformationSetPtr tsp( new TransformationSet() );
		//    if( inf != false ) {
		//	tsp = generate_options( tp );		        
		//    }
		//    else {
			tsp->insert( tp );
		    //}
		    
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
	    std::cerr << it->get<1>()-1 << " - " << trg_bytepos1 << std::endl;
	}
        
    }
      
    boost::posix_time::ptime pt_all_end = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta_all = pt_all_end - pt_start1;
    //boost::posix_time::time_duration delta_proc = pt_all_end - pt_start5;
    
    if(verbosity > 0) {
	//std::cerr << "Post loading processing took " << delta_proc.total_milliseconds() << " ms." << std::endl;
	std::cerr << "All transformations loaded in " << delta_all.total_milliseconds() << " ms." << std::endl;
    }

    return et;
}

//TransformationSetPtr RuleSet::generate_options( TransformationPtr &t ) {
//    TransformationSetPtr tsp( new TransformationSet() );
//    
//    //std::cerr << t->str() << std::endl;
//    
//    std::vector< std::vector<SymbolProb> > grid;
//    
//    int max = 1;
//    BOOST_FOREACH( Symbol s, *t->targets() ) {
//	if( s.is_nt() ) {
//	    std::vector<SymbolProb> v;
//	    v.push_back( SymbolProb( s, 0 ) );
//	    grid.push_back( v );
//	}
//	else {
//	    grid.push_back( inf->inflectAll( s, t->get_alignment() ) );
//	    max *= grid.back().size();
//	}
//    }
//    
//    for(int n = 0; n < max; n++) {
//	int m = n;
//	SListPtr new_targets( new SList() );
//	
//	double inflector_cost = 0;
//	BOOST_FOREACH( std::vector<SymbolProb> symbols, grid ) {
//	    int k = symbols.size();
//	    int i = m % k;
//	    
//	    new_targets->push_back( symbols[i].first );
//	    inflector_cost += symbols[i].second;
//	    
//	    m = ( m - i ) / k;
//	}
//	
//	double new_cost = t->get_cost() + inf->get_weight() * inflector_cost;
//	Floats new_unweighted = t->get_unweighted();
//	new_unweighted.push_back( inflector_cost );
//	
//	TransformationPtr ti( new Transformation( t->lhs(), t->sources(), new_targets, new_cost, new_unweighted, lmc ) );
//	ti->add_alignment( t->get_alignment() );
//	
//	//std::cerr << "\t" << ti->str() << std::endl;
//	tsp->insert( ti );
//    }
//    
//    if( tsp->size() == 0 ) {
//	Floats new_unweighted = t->get_unweighted();
//	new_unweighted.push_back( 0 );
//
//	TransformationPtr ti( new Transformation( t->lhs(), t->sources(), t->targets(), t->get_cost(), new_unweighted, lmc ) );
//	ti->add_alignment( t->get_alignment() );
//      
//	tsp->insert( ti );
//    }
//    
//    return tsp;
//}

TransformationPtr RuleSet::word_to_transformation(Symbol &lhs, SListPtr &srcSymbols, rules::Word &word, rules::Word &probs, rules::Word &align) {
   SListPtr trgSymbols ( new SList() );
   Floats costs;

   int non_terminals = 0;
    
   for(int i = 0; i < word.size(); i++) {
	rules::RuleSymbol rsym = trg_sym_map.get_symbol_by_number( word[i] );
	if(rsym.src_index() != -1) {
	    int i = srcSymbols->nt_index( rsym.src_index() );
	    if(i != -1) {
        	trgSymbols->push_back(srcSymbols->at(i));
        	non_terminals++;
	    }
	    else {
        	trgSymbols->push_back(Symbol(rsym.get_labels(), Range(-1,-1)));
	    }
	}
        else {
            trgSymbols->push_back(Symbol(rsym.get_labels(), Range(-1,-1)));
        }
    }
     
   double cost = 0;
   int i, j;
   for(i = 0, j = 0; j < tm_weights.size(); i++, j++) { // only tm_weight.size() costs are used 
       if(i < probs.size()) {                                                                                                 
            try {                                                                                                             
                costs.push_back(boost::lexical_cast<float>( trg_sym_map.get_symbol_by_number(probs[i]).label() ));            
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
   
   for(int i = 0; i < rs_vector.size(); i++) {
      costs.push_back(rs_vector[i]);
      cost += rs_weights[i] * rs_vector[i];
   }
   
   AlignmentPtr a( new Alignment );
   for(int i = 0; i < align.size(); i+=2) {
	a->insert(AlignmentPoint(align[i], align[i+1]));
   }
   
   TransformationPtr t( new Transformation(lhs, srcSymbols, trgSymbols, cost, costs, lmc) );
   t->add_alignment(a);
   
   //std::cerr << t->str() << std::endl;
   return t;
}

//
//TransformationPtr RuleSet::word_to_transformation2(Symbol &lhs, SListPtr &srcSymbols, rules::Word &w) {
//   SListPtr trgSymbols ( new SList() );
//   Floats costs;
//
//   int non_terminals = 0;
//    
//   for(int i = 0; i < w.size() - cost_length; i++) {
//	rules::RuleSymbol rsym = trg_sym_map.get_symbol_by_number( w[i] );
//	if(rsym.src_index() != -1) {
//	    int i = srcSymbols->nt_index( rsym.src_index() );
//	    if(i != -1) {
//        	trgSymbols->push_back(srcSymbols->at(i));
//        	non_terminals++;
//	    }
//	    else {
//        	trgSymbols->push_back(Symbol(rsym.get_labels(), Range(-1,-1)));
//	    }
//	}
//        else {
//            trgSymbols->push_back(Symbol(rsym.get_labels(), Range(-1,-1)));
//        }
//    }
//     
//   double cost = 0;
//   int i, j;
//   for(i = w.size() - cost_length, j = 0; j < tm_weights.size(); i++, j++) { // only tm_weight.size() costs are used 
//       if(i < w.size())
//	  costs.push_back(boost::lexical_cast<float>( trg_sym_map.get_symbol_by_number(w[i]).label() ));
//       else
//	  costs.push_back(0);
//	  
//       cost += (tm_weights[j] * costs[j]);
//   }
//   costs.push_back(trgSymbols->size() - non_terminals); // transformation length in terminal symbols
//   cost += ( word_penalty_weight * costs.back() );
//   
//   Floats rs_vector;
//   rs_vector.resize(rs_weights.size(), 0);
//   rs_vector[rule_set_index] = 1;
//   
//   for(int i = 0; i < rs_vector.size(); i++) {
//      costs.push_back(rs_vector[i]);
//      cost += rs_weights[i] * rs_vector[i];
//   }
//   
//   TransformationPtr t( new Transformation(lhs, srcSymbols, trgSymbols, cost, costs, lmc) );
//   //std::cerr << t->str() << std::endl;
//   return t;
//}

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
            
            if(path->size() == w.size()-1 and act == w.size()-1 and sym.end() == lhs.end()) {
		sl.push_back(std::make_pair(lhs, path));
		break;
            }
            
	    if( path->size() <= w.size() and act < w.size()-1) {
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

rules::SimpleDAG RuleSet::parse_to_dag(ParseGraphPtr &pg, Unmapper &unmap) {
    rules::SimpleDAG ndag;
    ParseGraph::Graph* g = pg->getBoostGraph();
    ParseGraph::TransitionMap map = boost::get(boost::edge_bundle, *g);
    std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> p = boost::edges(*g);
    
    
    while(p.first != p.second) {
	ParseGraph::Edge e = *p.first;
	if(map[e].isLexical() == false) {
	    Symbol sym( Label(map[e].getSymbol()), Range( map[e].getStart(), map[e].getEnd() ), !map[e].isLexical() );
	    rules::SimpleDAG tdag = subparse_to_dag(sym, map[e].getDepth(), pg, unmap);
    
	    ndag.nd_union(tdag);
	}
	p.first++;
    }
   
    ndag = prune_by_intersector(ndag);
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

rules::SimpleDAG RuleSet::subparse_to_dag(Symbol &lhs, int depth, ParseGraphPtr &pg, Unmapper &unmap) {
    rules::SimpleDAG ndag;

    ParseGraph::Graph* g = pg->getBoostGraph();
    rules::Symbol symbol = src_sym_map.get_number_by_string(lhs.label());
    
    if(unmap.count(symbol) == 0) {
        SymbolSet t;
        unmap[symbol] = t;
    }
    //std::cerr << "inserting " << symbol << " - " << lhs.str() << std::endl;
    unmap[symbol].insert(lhs);
    
    if(lhs.is_nt()) 
	nts.insert(symbol);
    
    std::map<int,rules::State> mapper;
    
    int q0 = ndag.new_state(true);
    int q1 = ndag.new_state(false);
    ndag.new_arc(q0, q1, symbol, 0);
    
    mapper[lhs.start()] = q1;
    
    ParseGraph::TransitionMap map = boost::get(boost::edge_bundle, *g);
    std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> p = boost::edges(*g);
    
    while(p.first != p.second) {
	ParseGraph::Edge e = *p.first;
	Symbol sym( Label(map[e].getSymbol()), Range( map[e].getStart(), map[e].getEnd() ), !map[e].isLexical() );
	
	if(lhs.start() <= sym.start() and lhs.end() >= sym.end()) {
	    if(lhs.start() < sym.start() or lhs.end() > sym.end() or depth > map[e].getDepth()) {
		if(mapper.count(sym.start()) == 0) {
		    rules::State t = ndag.new_state(false);
		    mapper[sym.start()] = t;
		}
		rules::State ns = mapper[sym.start()];
		
		if(mapper.count(sym.end()) == 0) {
		    rules::State t = ndag.new_state(false);
		    mapper[sym.end()] = t;
		    if(lhs.end() == sym.end())
			ndag.set_end_state(t);
		}
		rules::State nt = mapper[sym.end()];
		
		rules::Symbol a = src_sym_map.get_number_by_string(sym.label());
		if(a != -1) {
		    if(unmap.count(a) == 0) {
			SymbolSet t;
			unmap[a] = t;
		    }
		    //std::cerr << "inserting " << a << " - " << sym.str() << std::endl;
		    unmap[a].insert(sym);
		    
		    if(sym.is_nt()) { 
			nts.insert(a);
		    }
		    ndag.new_arc(ns,nt,a,0);
		}
	    }
	}
	p.first++;
    }
    return ndag;
}

void RuleSet::build_intersector() {
//    std::cerr << "n: " << max_length << " - k: " << max_nt << std::endl;

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

rules::SimpleDAG RuleSet::partial(int n, std::vector<int> &a) {
    rules::SimpleDAG ndag;
    rules::State q00 = ndag.new_state(true);
    rules::State q0 = ndag.new_state(false);
    ndag.new_arc(q00,q0,1,0);
    
    rules::State q = q0;
    for(int i = 0, j = 0; i < 7; i++) {
	rules::State p = ndag.new_state(false);
	ndag.set_end_state(p);
	
	ndag.new_arc(q, p, 0, 0);
	if(j < a.size() and a[j] == i+1) {
	    ndag.new_arc(q, p, 1, 0);
	    j++;
	}
	q = p;
    }
    
    return ndag;
}

bool RuleSet::nextksb(std::vector<int> &a, int &h, int &m,  int k, int n) {
    
    if(k == 0)
	return false;
	 
    bool first = false;
    
    if(a.size() != k) {
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


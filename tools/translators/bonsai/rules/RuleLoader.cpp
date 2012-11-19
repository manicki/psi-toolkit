#include "RuleLoader.hpp"

namespace poleng
{

namespace bonsai
{

RuleLoader::RuleLoader(std::string path, int max_length_, int max_nt_, LmContainerPtr lmc_) 
 : max_length(max_length_), max_nt(max_nt_), lmc(lmc_), max_trans_hyper(20), max_hyper_sym(20), eps(-1), verbosity(0) {
   
   RuleSetPtr rp(new RuleSet(path, max_length, max_nt, rs.size(), lmc) );
   rs.push_back(rp);
}

RuleLoader::RuleLoader(int max_length_, int max_nt_, LmContainerPtr lmc_) 
 : max_length(max_length_), max_nt(max_nt_), lmc(lmc_), max_trans_hyper(20), max_hyper_sym(20), eps(-1), verbosity(0) {}
    
void RuleLoader::add_rule_set(std::string path) {
   RuleSetPtr rp(new RuleSet(path, max_length, max_nt, rs.size(), lmc) );
   rs.push_back(rp);
}

void RuleLoader::add_rule_set( std::string path, SymInflectorPtr inf_ ) {
   RuleSetPtr rp(new RuleSet(path, max_length, max_nt, rs.size(), lmc, inf_) );
   rs.push_back(rp);
}

void RuleLoader::add_rule_set( RuleSetPtr rp ) {
   rs.push_back(rp);
}

EdgeTransformationsPtr RuleLoader::get_edge_transformations(ParseGraphPtr& pg) {
   if(rs.size() == 1) {
      EdgeTransformationsPtr local = rs[0]->get_edge_transformations(pg);
      fill_empty(pg, local);
      return local;
   }
   else if(rs.size() > 1) {
      EdgeTransformationsPtr merged( new EdgeTransformations() );
      for(int i=0; i < rs.size(); i++) {
         EdgeTransformationsPtr local = rs[i]->get_edge_transformations(pg);
         merge_edge_transformations(merged, local, i);
      }
      fill_empty(pg, merged);
      return merged;
   }
   else {
      return EdgeTransformationsPtr( new EdgeTransformations() );
   }
}

void RuleLoader::fill_empty(ParseGraphPtr &pg, EdgeTransformationsPtr &et) {
    ParseGraph::Graph* g = pg->getBoostGraph();
    ParseGraph::TransitionMap map = boost::get(boost::edge_bundle, *g);
    
    std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> p = boost::edges(*g);
    
    while(p.first != p.second) {
	ParseGraph::Edge e = *p.first;
        if(!map[e].isLexical()) {
            Symbol sym( Label(map[e].getSymbol()), Range( map[e].getStart(), map[e].getEnd() ), !map[e].isLexical() );
            if(et->count(sym) == 0) {
                (*et)[sym] = HyperEdgeSetPtr( new HyperEdgeSet() );
                if(verbosity > 0)
                    std::cerr << "No rules for " << sym.str() << std::endl;
                
                SListPtr target_list = pg->shallow_subpath(sym);
                if(verbosity > 0)
                    std::cerr << "Adding: " << target_list->str() << std::endl;
                
                HyperEdgePtr he( new HyperEdge(target_list) );
                
                Floats costs;
                costs.resize(tm_weights.size(), 10);
		double cost;
                for(int i=0; i < tm_weights.size(); i++) 
                    cost += tm_weights[i]*costs[i];
		    
		costs.push_back(0);                              // number of terminal words; not necessary to add word penalty to cost
                		
		Floats rs_vector;
		rs_vector.resize(rs_weights.size(), 1);
		
		for(int i = 0; i < rs_vector.size(); i++) {
		   costs.push_back(rs_vector[i]);
		   cost += rs_weights[i] * rs_vector[i];
		}
		
		// Here be dragons!
		costs.push_back( 0 );
		//
		
		TransformationPtr tp( new Transformation(sym, target_list, target_list, cost, costs, lmc) );
		
		AlignmentPtr ap( new Alignment() );
		for(int i = 1; i <= target_list->size(); i++)
		    ap->insert(AlignmentPoint(i,i));
		tp->add_alignment(ap);
		
                he->add(tp);
                (*et)[sym]->insert(he);
            }
        }
	p.first++;
    }
}

void RuleLoader::merge_edge_transformations(EdgeTransformationsPtr& result, EdgeTransformationsPtr& next, int index) {
   for(EdgeTransformations::iterator sym_it = next->begin(); sym_it != next->end(); sym_it++) {
      Symbol sym = sym_it->first;
      //std::cerr << "Merging from second rules set: " << sym.str() << std::endl;
      
      HyperEdgeSetPtr next_hset = sym_it->second;
      
      if(result->count(sym) > 0) {
         //std::cerr << "Real merging" << std::endl;
         
         HyperEdgeSetPtr result_hset = (*result)[sym];
         
         for(HyperEdgeSet::iterator hit1 = next_hset->begin(); hit1 != next_hset->end(); hit1++) {
            HyperEdgeSet::iterator found = result_hset->end();
            
            //std::cerr << "Searching for equivalent hyper edge" << std::endl;
            for(HyperEdgeSet::iterator hit2 = result_hset->begin(); hit2 != result_hset->end(); hit2++) {
               if((*hit1)->nts()->str() == (*hit2)->nts()->str())
                  found = hit2;
            }
            
            if(found != result_hset->end()) {
               //std::cerr << "Found" << std::endl;
               HyperEdgePtr h = *found;
               result_hset->erase(found);
               merge_hyper_edge(h, *hit1);
               result_hset->insert(h);
            }
            else {
               //std::cerr << "Not found, just adding" << std::endl;
               result_hset->insert(*hit1); // insert new HyperEdge from next HyperEdgeSet 
            }
         }
      }
      else {
         //std::cerr << "Just copying" << std::endl;
         (*result)[sym] = HyperEdgeSetPtr( new HyperEdgeSet() );
         (*result)[sym]->insert(next_hset->begin(), next_hset->end());
      }
   }
}

void RuleLoader::merge_hyper_edge(HyperEdgePtr &result, HyperEdgePtr next) {
   TransformationSetPtr next_tr = next->get_transformations();
   TransformationSetPtr result_tr = result->get_transformations();
   //std::cerr << "Merging single hyper edge ... ";
   for(TransformationSet::iterator it = next_tr->begin(); it != next_tr->end(); it++) {
      //std::cerr << "Inserting: " << (*it)->str() << std::endl; 
      result_tr->insert(*it);
   }
   //result_tr->insert(next_tr->begin(), next_tr->end());
   //std::cerr << "finished" << std::endl;
}


void RuleLoader::set_verbosity(int val) {
   for(std::vector<RuleSetPtr>::iterator it = rs.begin(); it != rs.end(); it++)
      (*it)->set_verbosity(val);
}

void RuleLoader::set_max_transformations_per_hyperedge(int val) {
   for(std::vector<RuleSetPtr>::iterator it = rs.begin(); it != rs.end(); it++)
      (*it)->set_max_transformations_per_hyperedge(val);
}

void RuleLoader::set_max_hyperedges_per_nonterminal(int val) {
   for(std::vector<RuleSetPtr>::iterator it = rs.begin(); it != rs.end(); it++)
      (*it)->set_max_hyperedges_per_nonterminal(val);
}

void RuleLoader::set_max_transformation_factor(double val) {
   for(std::vector<RuleSetPtr>::iterator it = rs.begin(); it != rs.end(); it++)
      (*it)->set_max_transformation_factor(val);
}

Floats RuleLoader::tm_weights;
Floats RuleLoader::lm_weights;
Floats RuleLoader::rs_weights;
double RuleLoader::word_penalty_weight = 0;

}

}


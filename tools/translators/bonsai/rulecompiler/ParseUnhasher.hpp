#ifndef PARSEUNHASHER_HPP__
#define PARSEUNHASHER_HPP__

#include <sstream>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>

#include "ParseGraph.hpp"

#include "CompressedDAG.hpp"
#include "SimpleDAG.hpp"
#include "RuleSymbolNumberMap.hpp"

namespace poleng {

    namespace bonsai {
    
        class ParseUnhasher {
          public:
            ParseUnhasher(std::string fsafile, std::string symfile) : hasSymbolMap(true), gap_string("<X>"), gap_num(0) {
                src_fsa.open(fsafile, 0);
                src_sym_map.open(symfile);
            }
            
            ParseUnhasher(std::string fsafile) : hasSymbolMap(false), gap_string("<X>") {
                src_fsa.open(fsafile, 0);
            }
            
            void set_gap_string(std::string gap_string_) {
                gap_string = gap_string_;
            }
            
            void set_gap_num(int gap_num_) {
                gap_num = gap_num_;
            }
            
            /*void find(std::vector<std::string> &t) {
                rules::SimpleDAG dag = sentence_to_dag(t);
                dag.minimize();
                rules::WordList wordlist = src_fsa.multihash(dag);
                
                for(rules::WordList::iterator it = wordlist.begin(); it != wordlist.end(); it++) {
                    if(it != wordlist.begin()) {
                        std::cout << " ";
                    }
                    std::cout << it->get<1>(); 
                }
                std::cout << std::endl;
            }
            */
            
            std::set<int> find(std::vector<std::string> &t) {
                rules::SimpleDAG dag = sentence_to_dag(t);
                dag.minimize();
                rules::WordList wordlist = src_fsa.multihash(dag);
                
                std::set<int> hashes;
                for(rules::WordList::iterator it = wordlist.begin(); it != wordlist.end(); it++)
                    hashes.insert(it->get<1>());
                return hashes;
            }
            
            /*void find(std::vector<int> &t) {
                rules::SimpleDAG dag = sentence_to_dag(t);
                dag.minimize();
                
                rules::WordList wordlist = src_fsa.multihash(dag);
                
                for(rules::WordList::iterator it = wordlist.begin(); it != wordlist.end(); it++) {
                    if(it != wordlist.begin()) {
                        std::cout << " ";
                    }
                    std::cout << it->get<1>(); 
                }
                std::cout << std::endl;
            }
            */

            std::set<int> find(std::vector<int> &t) {
                rules::SimpleDAG dag = sentence_to_dag(t);
                dag.minimize();
                
                rules::WordList wordlist = src_fsa.multihash(dag);
                
                std::set<int> hashes;
                for(rules::WordList::iterator it = wordlist.begin(); it != wordlist.end(); it++)
                    hashes.insert(it->get<1>());
                return hashes;
            }
            
            //void find(ParseGraphPtr &pg) {
            //    rules::SimpleDAG dag = parse_to_dag(pg);
            //    dag.minimize();
            //    
            //    rules::WordList wordlist = src_fsa.multihash(dag);
            //    
            //    for(rules::WordList::iterator it = wordlist.begin(); it != wordlist.end(); it++) {
            //        if(it != wordlist.begin()) {
            //            std::cout << " ";
            //        }
            //        std::cout << it->get<1>(); 
            //    }
            //    std::cout << std::endl;
            //}

            std::set<int> find(ParseGraphPtr &pg) {
                rules::SimpleDAG dag = parse_to_dag(pg);
                dag.minimize();
                
                rules::WordList wordlist = src_fsa.multihash(dag);
                
                std::set<int> hashes;
                for(rules::WordList::iterator it = wordlist.begin(); it != wordlist.end(); it++)
                    hashes.insert(it->get<1>());
                return hashes;
            }
            
          private:    
            rules::CompressedDAG src_fsa;
            rules::RuleSymbolNumberMap src_sym_map;
            
            std::string gap_string;
            int gap_num;
            bool hasSymbolMap;
            
            rules::SimpleDAG sentence_to_dag(std::vector<std::string> &t) {
                rules::SimpleDAG ndag;                                                                                                    
                
                int start = ndag.new_state(true);
                
                // Create linear automaton for whole sentence
                
                int q1 = start, q2;
                for(std::vector<std::string>::iterator it = t.begin(); it != t.end(); it++) {
                    q2 = ndag.new_state(false);
                    rules::Symbol symbol = src_sym_map.get_number_by_string( *it );
                    //std::cerr << symbol << std::endl;
                    ndag.new_arc(q1, q2, symbol, 0);
                    q1 = q2;
                }
                int end = q2;
                
                ndag.set_end_state(end);
                
                // Create edges for all substrings
                
                for(int i = 2; i < end; i++) {
                    rules::Symbol symbol = src_sym_map.get_number_by_string( t[i-1] );
                    ndag.new_arc(start, i, symbol, 0);
                }
                for(int i = end-2; i > start; i--) {
                    rules::Symbol symbol = src_sym_map.get_number_by_string( t[i] );
                    ndag.new_arc(i, end, symbol, 0);
                }
                
                // Add single edge for each token
                
                for(std::vector<std::string>::iterator it = t.begin(); it != t.end(); it++) {
                    rules::Symbol symbol = src_sym_map.get_number_by_string( *it );
                    ndag.new_arc(start, end, symbol, 0);
                }
                
                // Add gap symbols for all states
                
                rules::Symbol gap = src_sym_map.get_number_by_string( gap_string );
                for(int i = start; i < end; i++) {
                    for(int j = i+1; j <= end; j++) {
                        ndag.new_arc(i, j, gap, 0);
                    }
                }
                return ndag;
            }
            
            rules::SimpleDAG sentence_to_dag(std::vector<int> &t) {
                rules::SimpleDAG ndag;                                                                                                    
                
                int start = ndag.new_state(true);
                
                // Create linear automaton for whole sentence
                
                int q1 = start, q2;
                for(std::vector<int>::iterator it = t.begin(); it != t.end(); it++) {
                    q2 = ndag.new_state(false);
                    rules::Symbol symbol = *it;
                    ndag.new_arc(q1, q2, symbol, 0);
                    q1 = q2;
                }
                int end = q2;
                
                ndag.set_end_state(end);
                
                // Create edges for all substrings
                
                for(int i = 2; i < end; i++) {
                    rules::Symbol symbol = t[i-1];
                    ndag.new_arc(start, i, symbol, 0);
                }
                for(int i = end-2; i > start; i--) {
                    rules::Symbol symbol = t[i];
                    ndag.new_arc(i, end, symbol, 0);
                }
                
                // Add single edge for each token
                
                for(std::vector<int>::iterator it = t.begin(); it != t.end(); it++) {
                    rules::Symbol symbol = *it;
                    ndag.new_arc(start, end, symbol, 0);
                }
                
                // Add gap symbols for all states
                
                rules::Symbol gap = gap_num;
                for(int i = start; i < end; i++) {
                    for(int j = i+1; j <= end; j++) {
                        ndag.new_arc(i, j, gap, 0);
                    }
                }
                
                return ndag;
            }
            
            rules::SimpleDAG parse_to_dag(ParseGraphPtr &pg) {                                                  
                rules::SimpleDAG ndag;                                                                                                    
                ParseGraph::Graph* g = pg->getBoostGraph();                                                                               
                ParseGraph::TransitionMap map = boost::get(boost::edge_bundle, *g);                                                       
                std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> p = boost::edges(*g);                                                   
                
                while(p.first != p.second) {                                                                                              
                    ParseGraph::Edge e = *p.first;                                                                                        
                    if(map[e].isLexical() == false) {                                                                                     
                        Symbol sym( Label(map[e].getSymbol()), Range( map[e].getStart(), map[e].getEnd() ), !map[e].isLexical() );        
                        rules::SimpleDAG tdag = subparse_to_dag(sym, map[e].getDepth(), pg);                                       
                        ndag.nd_union(tdag);                                                                                              
                    }                                                                                                                     
                    p.first++;                                                                                                            
                }                                                                                                                         
                return ndag;                                                                                                              
            }                                                                                                                             
             
            rules::SimpleDAG subparse_to_dag(Symbol &lhs, int depth, ParseGraphPtr &pg) {
                rules::SimpleDAG ndag;
            
                ParseGraph::Graph* g = pg->getBoostGraph();
                rules::Symbol symbol = src_sym_map.get_number_by_string(lhs.label());
                
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
                                ndag.new_arc(ns,nt,a,0);
                            }
                        }
                    }
                    p.first++;
                }
                return ndag;
            }
                
        };
            
    }

}

#endif

#ifndef COMPRESSEDDAG_HPP__
#define COMPRESSEDDAG_HPP__

#include <iostream>
#include <fstream>
#include <limits>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <vector>
#include <map>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "SimpleDAG.hpp"

namespace poleng
{

namespace bonsai
{

namespace rules {
  
    const int S_RD = 0;  // flagi, S_RD|A_MM oznacza wczytaj stany do pamieci, mapuj przejścia.
    const int A_RD = 0;  // Warto dodać dodatkowe flagi, np. A_DYNMM (dynamiczne mapowanie) lub 
    const int S_MM = 1;  // A_DYNRD (dynamiczne wczytywanie) itp.
    const int A_MM = 2;
    
    class CompressedDAG {
      protected:
        FILE* sf;
        FILE* af;
        int mode;
        
        Position* states;
        Arc* arcs;
        
        Position* d_states;
        Arc* d_arcs;
        
        unsigned int sn;
        unsigned int an;
        
        unsigned int d_sn;
        
        unsigned int da_start;
        unsigned int da_length;
    
        char* achars;
        unsigned int achars_size; 
    
        inline State delta(State, Symbol);
        inline ArcIt linsearch(ArcIt, ArcIt, Symbol);
        inline ArcIt binsearch(ArcIt, ArcIt, Symbol);
        inline ArcIt linsearch_num(ArcIt, ArcIt, Weight);
        inline ArcIt binsearch_num(ArcIt, ArcIt, Weight);
        inline ArcIt find(State, Symbol);
        inline ArcIt find_num(State, int);
        inline void set_end_state(State);
        Position arc_no(Position);
        
        long filesize(FILE*);
        
        void access();
        
        Position* read_states(int, int);
        Arc* read_arcs(int, int);
        
        Position* map_states(int, int);
        Arc* map_arcs(int, int);
        void unmap_states();
        void unmap_arcs();
        
      public:  
        CompressedDAG();
        CompressedDAG(unsigned int, unsigned int);
        CompressedDAG(std::string, int);
        
        virtual Range row(State);
        bool is_end_state(State);
        
        bool in(Word &w);
        bool prefix(Word &w);
        int hash(Word &w);
        Word unhash(int);
      
        virtual void open(std::string, int);
        
        SimpleDAG intersect(SimpleDAG &);
        WordList multihash(SimpleDAG &);
        WordListHashed multiunhash(std::set<int> &);
    
        void print() {
            for(int i = 0; i < sn; i++) {
                Range r = row(i);
                for(ArcIt it = r.first; it != r.second; it++) {
                    std::cout << i << "\t" << it->q << "\t" << it->a << std::endl;
                }
                if(is_end_state(i)) {
                    std::cout << i << std::endl;
                }
            }
        }
            
    };


}

}

}

#endif

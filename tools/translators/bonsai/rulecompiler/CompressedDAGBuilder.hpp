#ifndef COMPRESSEDDAGBUILDER_H__
#define COMPRESSEDDAGBUILDER_H__

#include "CompressedDAG.hpp"
//#include <huffman/huffman.hpp>

namespace poleng
{

namespace bonsai 
{

namespace rules {

bool sort_pred(const std::pair<unsigned int, unsigned int> &left, const std::pair<unsigned int, unsigned int> &right) {
    return left.second > right.second or left.first < right.first;
}


class CompressedDAGBuilder : protected CompressedDAG {
  private:
    std::vector<unsigned int> stc;
    
    std::string filename;
    bool counting;

    //HuffmanWrapper hw;

    Range row(State i) {
	if(counting) {
	    if(stc.size() <= i) 
		stc.resize(i+1, 0);
	    stc[i]++;
	}
	return CompressedDAG::row(i);
    }    
    
    bool is_end_state(State p) {
	if(counting) {
	    if(stc.size() <= p) 
		stc.resize(p+1, 0);
	    stc[p]++;
	}
        return CompressedDAG::is_end_state(p);
    }
    
  public:
    CompressedDAGBuilder(bool c_) : counting(c_) {}
    CompressedDAGBuilder() : counting(false) {}
  
    template<class IStream>
    void compile(IStream &in, std::string filename, int mode) {
	std::ofstream of_states((filename + ".temp.s").c_str(), std::ios::binary);
	std::ofstream of_arcs((filename + ".temp.a").c_str(), std::ios::binary);
	
	build_from_text(in, of_states, of_arcs);
	
	of_states.close();
	of_arcs.close();
	
	//hw.generate_code_table((filename + ".temp.a").c_str());
	
	open(filename + ".temp", mode);
	sort_by_access_count(filename.c_str());
	
	std::remove((filename + ".temp.s").c_str());
	std::remove((filename + ".temp.a").c_str());
    }
  
    void write_access_list(char* filename) {
	std::ofstream out(filename, std::ios::out|std::ios::trunc);
	for(int i=0; i < stc.size(); i++) {
	    out << i << " - " << stc[i] << std::endl;
	} 
    }
    
    void rec_compute_word_numbers(State start, std::vector<Weight> &tn) {
	if(tn[start] == 0) {
	    Range r = CompressedDAG::row(start);
	    	    
	    for(ArcIt arcIt = r.first; arcIt != r.second; arcIt++) {
		arcIt->w = tn[start];
		rec_compute_word_numbers(arcIt->q, tn);
		tn[start] += tn[arcIt->q];
	    }
		
	    if(CompressedDAG::is_end_state(start)) {
		tn[start]++;
	    }
	}
    }

    
    void sort_by_access_count(const char* filename) {
	std::vector<Weight>* acc = new std::vector<Weight>();
	std::vector<Weight>* wno = new std::vector<Weight>();
	std::vector<Weight>* cbf = new std::vector<Weight>();
	
	std::cerr << "sn: " << sn << std::endl;
	
	wno->resize(sn);
	cbf->resize(sn);
	acc->resize(sn);
	for(int i = 0; i < sn; i++) {
	    (*wno)[i] = 0;
	    (*cbf)[i] = 0;
	    (*acc)[i] = 0;
	}
	
	std::cerr << "Computing word numbers" << std::endl;
	rec_compute_word_numbers(0,*wno);
	
	std::cerr << "Computing branching factors" << std::endl;
	(*cbf)[0] = 1;
	for(int i=0; i < cbf->size(); i++) {
	    Range r = CompressedDAG::row(i);
	    for(Arc* arcIt = r.first; arcIt < r.second; arcIt++)
		(*cbf)[arcIt->q] += (*cbf)[i];		    
	}
	
	unsigned int max = (*wno)[0];
	
	std::cerr << "Computing access counts" << std::endl;
	(*acc)[0] = (*wno)[0];
	for(int i=0; i < acc->size(); i++) {
	    Range r = CompressedDAG::row(i);
	    for(Arc* arcIt = r.first; arcIt < r.second; arcIt++)
		(*acc)[arcIt->q] += (*wno)[arcIt->q] * (*cbf)[i];
	}
	
	delete cbf;
	delete wno;
    
	std::cerr << "Creating list for sorting" << std::endl;
	
	std::list<std::pair<unsigned int, unsigned int> >* sorter = new std::list<std::pair<unsigned int, unsigned int> >();
	
	int i = sn-1;
	while(!acc->empty()) {
	    if(counting and stc.size() > i)
		sorter->push_front(std::make_pair( i, max + stc[i] ));
	    else
		sorter->push_front(std::make_pair( i, acc->back() ));
	    acc->pop_back();
	    i--;
	}
	
	delete acc;
	
	std::cerr << "Sorting list" << std::endl;
	sorter->sort(sort_pred);
	
	std::vector<State>* map = new std::vector<State>();
	std::vector<State>* pri = new std::vector<State>();
	
	map->resize(sn);
	i = 0;
	for(std::list<std::pair<unsigned int, unsigned int> >::iterator it = sorter->begin(); it != sorter->end(); it++) {
	    (*map)[it->first] = i++;
	    pri->push_back(it->first);
	}
	delete sorter;
    
	sort_and_save(filename, (State*)&((*map)[0]), (State*)&((*pri)[0]));
	
	delete map;
	delete pri;
	
	std::cerr << "Finished" << std::endl;
    }

    void sort_and_save(const char* filename, State* map, State* pri) {
	
	std::ofstream new_s(std::string(filename).append(".s").c_str(), std::ios::out|std::ios::binary|std::ios::trunc);
	std::ofstream new_a(std::string(filename).append(".a").c_str(), std::ios::out|std::ios::binary|std::ios::trunc);
	
	std::cerr << "Writing new automaton" << std::endl;
	
	int c = 0;
	for(int j=0; j < sn; j++) {
	    if(CompressedDAG::is_end_state(pri[j])) {
		int ec = c;
		ec |= (1 << (sizeof(Position)*8-1));
		new_s.write((char*)&ec,sizeof(Position));
	    }
	    else 
		new_s.write((char*)&c,sizeof(Position));
	    
	    Range r = row(pri[j]);
	    
	    int length = std::distance(r.first, r.second);
	    Arc* modarcs = new Arc[length];
	    memcpy(modarcs, r.first, sizeof(Arc)*length);
	    for(int i = 0; i < length; i++) {
		modarcs[i].q = map[modarcs[i].q];
		c++;
	    }
	    
	    //std::string huff_str = hw.encode_string((unsigned char*)modarcs,sizeof(Arc)*length);
	    //new_a.write((char*)huff_str.c_str(),huff_str.size());
	    new_a.write((char*)modarcs,sizeof(Arc)*length);
	    
	    delete [] modarcs;
	}
	
	new_s.close();
	new_a.close();
    }
    
    template <class IStream, class OStream1, class OStream2>
    void build_from_text(IStream &in, OStream1 &out_states, OStream2 &out_arcs) {
	using namespace std;
	using namespace boost;
	
	unsigned int state_c = 0;
	Position arc_c = 0;
	
	std::string line;
	while(getline(in, line)) {
	    std::vector<int> input;
	      
	    boost::regex re("(\\d+)");
	    boost::sregex_token_iterator it(line.begin(), line.end(), re, 1);
	    boost::sregex_token_iterator end;
	
	    while(it != end)
	      input.push_back(boost::lexical_cast<int>(*it++));
	
	    if(input.size() == 1) {   // found end state
		int state = input[0];
		if(state_c == state) {
		    Position temp_arc_c = arc_c;
		    temp_arc_c |= (1 << (sizeof(Position)*8-1));
		    out_states.write((char*)&temp_arc_c, sizeof(Position));
		    state_c++;
		}
	    }
	    else if(input.size() == 3) { // adding arc		
		int src_state = input[0];
		Arc arc;
		
		arc.q = input[1];
		arc.a = input[2];
		arc.w = 0;
		
		if(state_c == src_state) { // source does not exist yet, adding new source state
		    out_states.write((char*)&arc_c, sizeof(int));
		    state_c++;
		}
		
		out_arcs.write((char*)&arc, sizeof(Arc));
		arc_c++;
	    }
	    
	    if(arc_c % 100000 == 0) {
		cerr << arc_c << " arcs read" << endl;
	    }
	}
	cerr << state_c  << " states read" << endl;
	cerr << arc_c  << " arcs read" << endl;
    	cerr << "finished" << endl;
    }
};

}

}

}

#endif

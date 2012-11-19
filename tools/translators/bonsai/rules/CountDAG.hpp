#ifndef COUNTDAG_HPP__
#define COUNTDAG_HPP__

#include "CompressedDAG.hpp"

namespace poleng
{

namespace bonsai
{

namespace dag {

bool sort_pred(const std::pair<unsigned int, unsigned int> &left, const std::pair<unsigned int, unsigned int> &right) {
    return left.second > right.second or left.first < right.first;
}


class CountDAG : public CompressedDAG {
  private:
    std::vector<unsigned int> stc;
    bool counting;

    Range row(State i) {
	if(counting) {
	    if(stc.size() <= i) 
		stc.resize(i+1, 0);
	    stc[i]++;
	}
	//std::cerr << "test" << std::endl;
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
    CountDAG(std::string s, int i, bool c_) : CompressedDAG(s,i), counting(c_) {}
    CountDAG(std::string s, int i) : CompressedDAG(s,i), counting(false) {}
  
    void write_access_list(char* filename) {
	std::ofstream out(filename, std::ios::out|std::ios::trunc);
	for(int i=0; i < stc.size(); i++) {
	    out << i << " - " << stc[i] << std::endl;
	} 
    }
    
    void rec_compute_word_numbers(State start, std::vector<Weight> &tn) {
	if(tn[start] == 0) {
	    Range r = CompressedDAG::row(start);
	    
	    int d = std::distance(r.first, r.second);
	    Arc* temparcs = new Arc[d];
	    std::copy(r.first,r.second,temparcs);
	    
	    for(ArcIt arcIt = temparcs; arcIt != temparcs + d; arcIt++) {
		State t = arcIt->q;
		rec_compute_word_numbers(t, tn);
		tn[start] += tn[t];
	    }
		
	    if(CompressedDAG::is_end_state(start)) {
		tn[start]++;
	    }
	    delete temparcs;
	}
    }

    
    void sort_by_access_count(char* filename) {
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

    void sort_and_save(char* filename, State* map, State* pri) {
	
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
	    for(Arc* arcIt = r.first; arcIt < r.second; arcIt++) {
		Arc modarc;
		modarc.a = arcIt->a;
		modarc.w = arcIt->w;
		modarc.q = map[arcIt->q];
		new_a.write((char*)&modarc,sizeof(Arc));
		c++;
	    }
	}
	
	new_s.close();
	new_a.close();
    }    
};

}

}

}

#endif

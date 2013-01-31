#include "LmContainer.hpp"
#include <algorithm>
#include <string>

#define LN10	2.30258509299404568402
//#define LN10 1

namespace poleng
{

namespace bonsai
{

LanguageModel::LanguageModel(std::string input) : n(0), lc(false) {
     const char* inputc = input.c_str();
     inputfilestream inp(inputc);
     lm = new lmtable();
     lm->load(inp, inputc, NULL, 0);
     n = lm->maxlevel();
}

int LanguageModel::get_n() { return n; };

void LanguageModel::clear_memory() {
     lm->reset_caches();
     lm->reset_mmap();
}

double LanguageModel::get_plain_cost(SListPtr &in, int level) {
     ngram *ng = new ngram(lm->dict);
     
     double logProb = 0;
     
     std::string surface = in->surface(level);
     if(lc)
          std::transform(surface.begin(), surface.end(), surface.begin(), ::tolower);
     
     std::stringstream plain;
     plain << surface;   
     
     int bol; double bow;     
     while(plain >> *ng) {
          if(ng->size >= lm->maxlevel()) {
		ng->size = lm->maxlevel();
	    	double pr = lm->lprob(*ng,&bow,&bol) * LN10;
            	logProb += pr;
	   }
     }
     delete ng;
     return -1 * logProb;
}

double LanguageModel::get_front_cost(SListPtr &in, int level) {
     double logProb = 0;

     ngram *ng = new ngram(lm->dict);
     std::stringstream ss;
     
     for(int i = 0; i < std::min((int)in->size(), n-1); i++) {
          std::string surface = in->at(i).label(level);
          if(lc)
		std::transform(surface.begin(), surface.end(), surface.begin(), ::tolower);
          if(i > 0)
               ss << " ";
          ss << surface;
     }
     
     int bol; double bow;
     while(ss >> *ng) {
          if(ng->size >= lm->maxlevel()) ng->size = lm->maxlevel();
          double pr = lm->lprob(*ng,&bow,&bol) * LN10;
          logProb += pr;
     }
     delete ng;
     return -1 * logProb;
}

double LanguageModel::get_terminal_cost(SListPtr &in, int level) {
     double logProb = 0;

     ngram *ng = new ngram(lm->dict);
     std::stringstream ss;
     
     for(SList::iterator it = in->begin(); it != in->end(); it++) {
          if(it->is_nt()) {
               int bol; double bow;
               while(ss >> *ng) {
                    if(ng->size >= lm->maxlevel()) ng->size = lm->maxlevel();
                    double pr = lm->lprob(*ng,&bow,&bol) * LN10;
                    logProb += pr;
               }
               ss.str(std::string());
          }
          else {
               std::string surface = it->label(level);
               if(lc)
	    	    std::transform(surface.begin(), surface.end(), surface.begin(), ::tolower);
               if(ss.str() != "")
                    ss << " ";
               ss << surface;
          }
     }
     
     int bol; double bow;
     while(ss >> *ng) {
          if(ng->size >= lm->maxlevel()) ng->size = lm->maxlevel();
          double pr = lm->lprob(*ng,&bow,&bol) * LN10;
          logProb += pr;
     }
     ss.str(std::string());
     delete ng;
     
     return -1 * logProb;
}


double LanguageModel::get_cost(SListPtr &in, int level) {
     double logProb = 0;

     ngram *ng = new ngram(lm->dict);
     std::stringstream ss;
     
     std::string surface = in->surface(level);
     if(lc)
    	  std::transform(surface.begin(), surface.end(), surface.begin(), ::tolower);
     
     ss << surface;
     
     int bol; double bow;
     while(ss >> *ng) {
          if(ng->size >= lm->maxlevel()) {
               ng->size = lm->maxlevel();
               double pr = lm->lprob(*ng,&bow,&bol) * LN10;
               logProb += pr;
          }
     }
     delete ng;
     return -1 * logProb;
}


double LanguageModel::get_cost(SListPtr &in, LinkPoints &lp, int level) {
     double logProb = 0;
          
     //std::cerr << "SList: " << in->str() << std::endl;
     
     int last_end = n-2;
     for(LinkPoints::iterator lit = lp.begin(); lit != lp.end(); lit++) {
          double linkLogProb = 0;
          ngram *ng = new ngram(lm->dict);
          std::stringstream ss;
          
          int start = std::max(last_end-n+2, lit->second-n+1);
          int end   = std::min((int)in->size()-1, lit->first+n-1);
          last_end = end;
          
	  if(end - start + 1 >= n) {
               for(int i = start; i <= end; i++) {
                    if(i > start)
                         ss << " ";
                    std::string surface = in->at(i).label(level);
                    if(lc) std::transform(surface.begin(), surface.end(), surface.begin(), ::tolower);
                    ss << surface;
               }
                    
               int bol; double bow;
               while(ss >> *ng) {
                    if(ng->size >= lm->maxlevel()) {
                         ng->size = lm->maxlevel();
                         double pr = lm->lprob(*ng,&bow,&bol) * LN10;
                         linkLogProb += pr;
                    }
               }
               logProb += linkLogProb;
               
               //std::cerr << ss.str() << " - " << linkLogProb << std::endl;
          }
	  delete ng;
     }
     return -1 * logProb;
}

void LanguageModel::set_lc(bool val) {
    lc = val;
}

/************************ LmContainer ***********************/

// clear memory dla container;

LmContainer::LmContainer() {};

void LmContainer::add_lm(LanguageModelPtr lm) {
    lms.push_back(lm);
}

int LmContainer::size() {
    return lms.size();
}

double LmContainer::get_front_cost(SListPtr &input) {
    double w = 0;
    for(size_t i=0; i<lms.size(); i++) 
	w += lm_weights[i] * lms[i]->get_front_cost(input, i);
    return w;
}

double LmContainer::get_terminal_cost(SListPtr &input) {
    double w = 0;
    for(size_t i=0; i<lms.size(); i++) 
	w += lm_weights[i] * lms[i]->get_terminal_cost(input, i);
    return w;
}

std::string LmContainer::get_plain_cost(SListPtr &input) {
    std::stringstream ss;
    for(size_t i=0; i<lms.size(); i++) 
	ss << lms[i]->get_plain_cost(input, i) << " ";
    return ss.str();
}

double LmContainer::get_cost(SListPtr &input, Floats &unweighted) {
    double w = 0;
    unweighted.resize(lms.size());
    for(size_t i=0; i<lms.size(); i++) {
	unweighted[i] = lms[i]->get_cost(input, i);
	w += lm_weights[i] * unweighted[i];
    }
    return w;
}

double LmContainer::get_cost(SListPtr &input, Floats &unweighted, LinkPoints &lp) {
    double w = 0;
    unweighted.resize(lms.size());
    for(size_t i=0; i<lms.size(); i++) {
	unweighted[i] = lms[i]->get_cost(input, lp, i);
	w += lm_weights[i] * unweighted[i];
    }
    return w;
}

void LmContainer::clear_memory() {
    for(size_t i=0; i<lms.size(); i++)
	lms[i]->clear_memory();
}

Floats LmContainer::tm_weights;
Floats LmContainer::lm_weights;
Floats LmContainer::rs_weights;
double LmContainer::word_penalty_weight = 0;

}

}


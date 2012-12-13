#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "common/ParseGraph.hpp"
#include "rules/CompressedDAG.hpp"
#include "rulecompiler/ParseUnhasher.hpp" 
 
namespace po = boost::program_options;

boost::mutex N_mutex;
boost::mutex corpus_mutex;
boost::mutex src_oc_mutex;
boost::mutex trg_oc_mutex;
boost::mutex cooc_mutex;
	
typedef unsigned long long int CountType; 

double logratio(CountType c_st, CountType c_s, CountType c_t, CountType N) {
    CountType o11 = c_st;
    CountType o12 = c_s - c_st;
    CountType o21 = c_t - c_st;
    CountType o22 = N - c_s - c_t + c_st;
    
    double e11 = (double)(o11 + o12)/N * (double)(o11 + o21)/N * N; 
    double e12 = (double)(o12 + o11)/N * (double)(o12 + o22)/N * N; 
    double e21 = (double)(o21 + o22)/N * (double)(o21 + o11)/N * N; 
    double e22 = (double)(o22 + o21)/N * (double)(o22 + o12)/N * N; 
    
    if(o11*e11*o12*e12*o21*e21*o22*e22 == 0)
	return 0;
    
    return 2* (
	o11*log(o11/e11) +
	o12*log(o12/e12) +
	o21*log(o21/e21) +
	o22*log(o22/e22)
    );
}

double log_fact(CountType n) {
    if (n <= 1)
	return 0;
    double pi = 3.14159265;
    return n * log(n) + 1/2 * log(2*pi*n) - n + 1/(12*n) - 1/(360*n*n*n);
}

double fisher(CountType c_st, CountType c_s, CountType c_t, CountType N) {
    CountType a = c_st;
    CountType b = c_s - c_st;
    CountType c = c_t - c_st;
    CountType d = N - c_s - c_t + c_st;
    
    double logp = -(
	log_fact(a+b) + log_fact(c+d) + log_fact(a+c) + log_fact(b+d)
	- log_fact(a) - log_fact(b) - log_fact(c) - log_fact(d) - log_fact(N)
    );
    
    return ((double)((int)(logp*1000))/1000);
}


typedef std::pair<int,int> IPair;

typedef std::vector<IPair> IPairs;
typedef std::vector<int> IPairsCounts;
typedef std::vector<int> SingleCounts;

typedef boost::shared_ptr<IPairs> IPairsPtr;
typedef boost::shared_ptr<IPairsCounts> IPairsCountsPtr;
typedef boost::shared_ptr<SingleCounts> SingleCountsPtr;
  
typedef boost::shared_ptr<CountType> IntPtr;
 
typedef boost::shared_ptr<poleng::bonsai::ParseUnhasher> ParseUnhasherPtr;
typedef boost::shared_ptr<std::ifstream> IfstreamPtr;  
  
class Collector {
  public:
    Collector(
        ParseUnhasherPtr src_,
	ParseUnhasherPtr trg_,
	
	IPairsPtr ipairs_,
	IPairsCountsPtr cooc_,
	SingleCountsPtr src_oc_,
	SingleCountsPtr trg_oc_,
	IntPtr N_,
	IntPtr C_,
	int max_sentences_   = -1,
	int beam_ = 20
    ) : src(src_), trg(trg_), ipairs(ipairs_), cooc(cooc_), src_oc(src_oc_), trg_oc(trg_oc_), N(N_), C(C_), max_sentences(max_sentences_), beam(beam_) {}

    void operator()(IfstreamPtr src_corpus, IfstreamPtr trg_corpus) {
	std::string src_line;
	std::string trg_line;
	
	void *src_ok, *trg_ok;
	{
	    boost::mutex::scoped_lock lock(corpus_mutex);
	    if(max_sentences != -1 and (*C) > max_sentences) {
		std::cerr << "test" << std::endl;
		src_ok = NULL;
		trg_ok = NULL;
	    }
	    else {
		src_ok = std::getline(*src_corpus, src_line);
		trg_ok = std::getline(*trg_corpus, trg_line);
	    }
	}
	
	int max = 50000000;
	while(src_ok && trg_ok) {
	    try {
	    
		src_pg.reset( new ParseGraph() );
		src_pg->load(src_line);
		
		trg_vec.clear();
		boost::sregex_token_iterator it(trg_line.begin(), trg_line.end(), boost::regex("\\s+"), -1);
		boost::sregex_token_iterator end;
		while(it != end)
		    trg_vec.push_back(*it++);
		
		std::set<int> src_nums = src->find(src_pg);    
		std::set<int> trg_nums = trg->find(trg_vec);
	
		{
		    boost::mutex::scoped_lock lock(N_mutex);
		    //*N += src_nums.size() * trg_nums.size();
		    (*N)++;
		}
		    
		for(std::set<int>::iterator src_it = src_nums.begin(); src_it != src_nums.end(); src_it++) {
		    if(*src_it < max and *src_it > 0) {
			boost::mutex::scoped_lock lock(src_oc_mutex);
			if(src_oc->size() <= *src_it) {
			    std::cerr << *src_it << std::endl;
			    src_oc->resize((*src_it)+1, 0);
			}
	
			//(*src_oc)[*src_it] += trg_nums.size();
			((*src_oc)[*src_it])++;
		    }
		}
		
		for(std::set<int>::iterator trg_it = trg_nums.begin(); trg_it != trg_nums.end(); trg_it++) {	
		    if(*trg_it < max and *trg_it > 0) {
			boost::mutex::scoped_lock lock(trg_oc_mutex);
			if(trg_oc->size() <= *trg_it)
			    trg_oc->resize((*trg_it)+1, 0);
	
			//(*trg_oc)[*trg_it] += src_nums.size();
			((*trg_oc)[*trg_it])++;
		    }
		}
		 
		IPairs::iterator pit = ipairs->begin();
		for(std::set<int>::iterator src_it = src_nums.begin(); src_it != src_nums.end(); src_it++) {
		    if(*src_it < max and *src_it > 0) { 
			IPair start(*src_it, 0);
			
			pit = std::lower_bound(pit, ipairs->end(), start);
			while(pit != ipairs->end() && pit->first == *src_it) {
			    if(trg_nums.count(pit->second)) {
				int i = std::distance(ipairs->begin(), pit);
				boost::mutex::scoped_lock lock(cooc_mutex);
				(*cooc)[i]++;
			    }
			    pit++;
			}
		    }
		}
	    
		{
		    boost::mutex::scoped_lock lock(corpus_mutex);
		    (*C)++;
		    
		    if((*C) % 1000 == 0) {
			std::cerr << "Processed " << (*C) << " corpus lines." << std::endl; 
		    }
		    if(max_sentences != -1 and (*C) > max_sentences) {
			src_ok = NULL;
			trg_ok = NULL;
		    }
		    else {
			src_ok = std::getline(*src_corpus, src_line);
			trg_ok = std::getline(*trg_corpus, trg_line);
		    }
		}
	    }
	    catch (...) {
		std::cerr << "An exception was caught." << std::endl;
		src_ok = std::getline(*src_corpus, src_line);
		trg_ok = std::getline(*trg_corpus, trg_line);
	    }
	}   
    }
    
  private:
    ParseUnhasherPtr src;
    ParseUnhasherPtr trg;
    
    IPairsPtr ipairs;
    IPairsCountsPtr cooc;
    SingleCountsPtr src_oc;
    SingleCountsPtr trg_oc;
    IntPtr N;
    IntPtr C;
    int max_sentences;
    int beam;
    
    poleng::bonsai::ParseGraphPtr src_pg;
    std::vector<std::string> trg_vec;
};

int main(int argc, char* args[]) {
    po::options_description desc("Allowed options");
    
    desc.add_options()
	("help", "Display this message")
	("hashed_rules", po::value<std::string>(), "Read rule numbers from text file arg")
	("src_corpus", po::value<std::string>(), "Read source corpus from text file arg")
	("trg_corpus", po::value<std::string>(), "Read target corpus from text file arg")
	("src_dag", po::value<std::string>(), "Read source DAG from binary file arg")
	("src_sym", po::value<std::string>(), "Read source Symbol Map from binary file arg")
	("trg_dag", po::value<std::string>(), "Read target DAG from binary file arg")
	("trg_sym", po::value<std::string>(), "Read target Symbol Map from binary file arg")
	("parallel,p", po::value<int>()->default_value(2), "number of parallel processes")
	("corpus_limit,c", po::value<int>()->default_value(-1), "maximum number of corpus sentences read in (-1 infinite)")
	("rule_limit,r", po::value<int>()->default_value(-1), "maximum number of rules read in (-1 infinite)")
	("counts", "Output all counts")
    ;
			    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, args, desc), vm);
    po::notify(vm);
					
    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }	
    
    if(not vm.count("hashed_rules")) {
	std::cout << "Missing argument 'hashed_rules'" << std::endl;
        std::cout << desc << std::endl;
        return 1;		
    }
    if(not vm.count("src_corpus")) {
	std::cout << "Missing argument 'src_corpus'" << std::endl;
        std::cout << desc << std::endl;
        return 1;		
    }
    if(not vm.count("trg_corpus")) {
	std::cout << "Missing argument 'trg_corpus'" << std::endl;
        std::cout << desc << std::endl;
        return 1;		
    }
    if(not vm.count("src_dag")) {
	std::cout << "Missing argument 'src_dag'" << std::endl;
        std::cout << desc << std::endl;
        return 1;		
    }
    if(not vm.count("trg_dag")) {
	std::cout << "Missing argument 'trg_dag'" << std::endl;
        std::cout << desc << std::endl;
        return 1;		
    }
    if(not vm.count("src_sym")) {
	std::cout << "Missing argument 'src_sym'" << std::endl;
        std::cout << desc << std::endl;
        return 1;		
    }
    if(not vm.count("trg_sym")) {
	std::cout << "Missing argument 'trg_sym'" << std::endl;
        std::cout << desc << std::endl;
        return 1;		
    }

    std::ifstream hash_numbers(vm["hashed_rules"].as<std::string>().c_str());
    std::string line;
    
    IPairsPtr rules(new IPairs());
    IPairsCountsPtr cooc(new IPairsCounts());
    while(std::getline(hash_numbers, line)) {
	boost::sregex_token_iterator it(line.begin(), line.end(), boost::regex("\\t+"), -1);
	boost::sregex_token_iterator end;
	int src_num = boost::lexical_cast<int>(*it++);
	int trg_num = boost::lexical_cast<int>(*it++);
	
	IPair p(src_num, trg_num);
	rules->push_back(p);
	cooc->push_back(0);
	
	if(rules->size() % 1000000 == 0)
	    std::cerr << "Read in " << rules->size() << " hashed pairs." << std::endl;
	    
	if(vm["rule_limit"].as<int>() != -1 && rules->size() >= vm["rule_limit"].as<int>())
	  break;
	    
    }

    IntPtr C(new CountType(0));
    IntPtr N(new CountType(0));
    SingleCountsPtr src_oc(new SingleCounts());
    SingleCountsPtr trg_oc(new SingleCounts());

    ParseUnhasherPtr src_pun(new poleng::bonsai::ParseUnhasher(vm["src_dag"].as<std::string>(), vm["src_sym"].as<std::string>()));    
    ParseUnhasherPtr trg_pun(new poleng::bonsai::ParseUnhasher(vm["trg_dag"].as<std::string>(), vm["trg_sym"].as<std::string>()));

    Collector collect(src_pun, trg_pun, rules, cooc, src_oc, trg_oc, N, C, vm["corpus_limit"].as<int>());
    
    IfstreamPtr src_corpus(new std::ifstream(vm["src_corpus"].as<std::string>().c_str()));
    IfstreamPtr trg_corpus(new std::ifstream(vm["trg_corpus"].as<std::string>().c_str()));
    
    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
    int max = vm["parallel"].as<int>();

    typedef boost::shared_ptr<boost::thread> ThreadPtr;
    std::vector<ThreadPtr> thread_vec;
    for(int i = 0; i < max; i++) {
	ThreadPtr t(new boost::thread(collect, src_corpus, trg_corpus));
	thread_vec.push_back(t);
    }
    for(int i = 0; i < max; i++) {
	thread_vec[i]->join();
    }
    
    boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta = pt_end - pt_start;
    int d = delta.total_milliseconds();
    
    std::cerr << d << " ms in total." << std::endl;
    
    for(IPairs::iterator it = rules->begin(); it != rules->end(); it++) {
	IPair p = *it;
	int i = std::distance(rules->begin(), it);
	//std::cout << fisher((*cooc)[i], (*src_oc)[p.first], (*trg_oc)[p.second], *N);
	//if(vm.count("counts"))
	std::cout << (*cooc)[i] << "\t" << (*src_oc)[p.first] << "\t" << (*trg_oc)[p.second] << "\t" << *N;
	std::cout << std::endl;
    }
}


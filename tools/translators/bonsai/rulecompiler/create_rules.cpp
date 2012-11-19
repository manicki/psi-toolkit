#include <string>
#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include "RuleGenerator.hpp"

typedef boost::shared_ptr<std::istream> IfstreamPtr;  
typedef boost::shared_ptr<std::ostream> OfstreamPtr;  
typedef boost::shared_ptr<int> IntPtr;  

using namespace poleng::bonsai;

boost::mutex corpus_mutex;
boost::mutex stdout_mutex;

static int C = 0;
static int OFFSET = 0;
static int LIMIT  = 0;

class Generator {
    private:
        boost::program_options::variables_map vm;
        std::map<std::string, std::set<std::string> > fm;
        
    public:    
        Generator(boost::program_options::variables_map vm_, std::map<std::string, std::set<std::string> > fm_) : vm(vm_), fm(fm_) {}
        
        
        void operator()(IfstreamPtr src_corpus, IfstreamPtr trg_corpus, IfstreamPtr alignments, OfstreamPtr out, OfstreamPtr log) {
            RectGenerator rectGen(fm);
            RuleGenerator ruleGen;
            
            ruleGen.setLimitSymbols(vm["limit-symbols"].as<int>());
            ruleGen.setLimitNonTerminals(vm["limit-nonterminal-symbols"].as<int>());
            ruleGen.setLimitDepth(vm["limit-depth"].as<int>());
            ruleGen.setLimitCompositions(vm["limit-composition"].as<int>());
        
            if (vm.count("include-k"))
                ruleGen.includeCompositionLevel();
            if (vm.count("include-rects"))
                ruleGen.includeRects();
            if (vm.count("no-limits"))
                ruleGen.setNoLimits();
            if (vm.count("with-alignments"))
                ruleGen.setIncludeAlignments();
            if (vm.count("use-alignments"))
                ruleGen.setUseWordAlignments();
            
            std::string src_serial;
            std::string trg_text;
            std::string aln_text;
            
            void *src_ok, *trg_ok, *aln_ok;
            {
                boost::mutex::scoped_lock lock(corpus_mutex);
                if(LIMIT != 0 and C >= LIMIT + OFFSET) {
                    src_ok = NULL;
                    trg_ok = NULL;
                    aln_ok = NULL;
                }
                else {
                    src_ok = std::getline(*src_corpus, src_serial);
                    trg_ok = std::getline(*trg_corpus, trg_text);
                    aln_ok = std::getline(*alignments, aln_text);
                }
               
                C++;
                if(log != false) {
                    log->seekp(0);
                    (*log) << C << std::endl;
                }
            }
            
            while(src_ok && trg_ok && aln_ok) {
                
                ParseGraphPtr src_pg( new ParseGraph(src_serial) );
                std::string src_text = src_pg->write_tokens();
                
                try {
                    std::ostream &output = (out != false) ? (*out) : std::cout ; 
                        
                    output << "# L1: " << src_text << std::endl;
                    output << "# L2: " << trg_text << std::endl;
                    output << "# Alignment: " << aln_text << std::endl << std::endl;
                        
                    std::vector<RectCoord>   rects = rectGen.generate(src_text, trg_text, aln_text, src_pg);
                    std::vector<std::string> rules = ruleGen.generateRules(src_pg, rects, trg_text, aln_text);
                    
                    {
                        boost::mutex::scoped_lock lock(stdout_mutex);    
                        std::ostream &output = (out != false) ? (*out) : std::cout ; 
                        
                        output << "# L1: " << src_text << std::endl;
                        output << "# L2: " << trg_text << std::endl;
                        output << "# Alignment: " << aln_text << std::endl << std::endl;
                        
                        BOOST_FOREACH( std::string rule, rules )
                            output << rule << std::endl;
                            
                        output << std::endl;
                    }
                    {
                        boost::mutex::scoped_lock lock(corpus_mutex);
                        if(LIMIT != 0 and C >= LIMIT + OFFSET) {
                            src_ok = NULL;
                            trg_ok = NULL;
                            aln_ok = NULL;
                        }
                        else {
                            src_ok = std::getline(*src_corpus, src_serial);
                            trg_ok = std::getline(*trg_corpus, trg_text);
                            aln_ok = std::getline(*alignments, aln_text);
                        }
                        
                        if(C % 1000 == 0)
                            std::cerr << "Generating rules for sentence: " << C << std::endl;
                            
                        C++;
                        if(log != false) {
                            log->seekp(0);
                            (*log) << C << std::endl;
                        }
                    }
                }
                catch(...) {
                    std::cerr << "An exception occurred" << std::endl;
                    
                    boost::mutex::scoped_lock lock(corpus_mutex);
                    if(LIMIT != 0 and C >= LIMIT + OFFSET) {
                        src_ok = NULL;
                        trg_ok = NULL;
                        aln_ok = NULL;
                    }
                    else {
                        src_ok = std::getline(*src_corpus, src_serial);
                        trg_ok = std::getline(*trg_corpus, trg_text);
                        aln_ok = std::getline(*alignments, aln_text);
                    }
                    
                    C++;
                    if(log != false) {
                        log->seekp(0);
                        (*log) << C << std::endl;
                    }
                }
                
            }
        }
};

int main(int argc, char **argv)
{
    boost::program_options::options_description desc("Options");
    
    desc.add_options()
        ("help", "prints this information")

        ("source_corpus,s", boost::program_options::value<std::string>(), "input filename")
        ("target_corpus,t", boost::program_options::value<std::string>(), "input filename")
        ("alignment,a", boost::program_options::value<std::string>(), "input filename")
        
        ("output,f", boost::program_options::value<std::string>(), "output file (uses STDOUT if omitted)")
        ("log", boost::program_options::value<std::string>(), "log file")
        ("resume", "resume from last entry in log file")
        
        ("offset-sentences,o", boost::program_options::value<int>()->default_value(0), "set corpus offset to arg")
        ("limit-sentences,l",  boost::program_options::value<int>()->default_value(0), "limit corpus to arg sentences")
    
        ("limit-symbols,n", boost::program_options::value<int>()->default_value(4), "symbols number limit")
        ("limit-nonterminal-symbols,m", boost::program_options::value<int>()->default_value(2), "nonterminal symbols number limit")
        ("limit-depth,d", boost::program_options::value<int>()->default_value(2), "nonterminal symbols depth limit")
        ("limit-composition,k", boost::program_options::value<int>()->default_value(10), "graphs composition level limit")

        ("include-k", "include k parameter in output")
        ("include-rects", "include rectangles in output")
        ("with-alignments", "include alignments in output")
        ("use-alignments", "use word alignments provided in input")    
        ("no-limits", "uses no limits except composition level limit")
        
        ("parallel,p", boost::program_options::value<int>()->default_value(2), "number of parallel processes")
    ;
    
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    std::string src_file;
    std::string trg_file;
    std::string aln_file;
    
    if(vm.count("source_corpus")) {
        src_file = vm["source_corpus"].as<std::string>();
    }
    else {
        std::cerr << "No source corpus file given" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }
    
    if(vm.count("target_corpus")) {
        trg_file = vm["target_corpus"].as<std::string>();
    }
    else {
        std::cerr << "No target corpus file given" << std::endl;
        std::cout << desc << std::endl;
        return 0;
        
    }
    
    if(vm.count("alignment")) {
        aln_file = vm["alignment"].as<std::string>();
    }
    else {
        std::cerr << "No alignment file given" << std::endl;
        std::cout << desc << std::endl;
        return 0;        
    }
    
    // tymczasowe ********
    
    std::map<std::string, std::set<std::string> > function_map;
    std::set<std::string> articles;
    
    articles.insert("el");
    articles.insert("la");
    articles.insert("un");
    articles.insert("una");
    articles.insert("los");
    articles.insert("las");
    articles.insert("del");
    articles.insert("de");
    
    //function_map["R"] = articles;
    //function_map["FR"] = articles;

    std::set<std::string> preps;
    preps.insert("a");
    preps.insert("al");
    preps.insert("de");
    preps.insert("del");
    preps.insert("para");
    preps.insert("por");
    
    //function_map["FPR"] = preps;

    // *******************

    IfstreamPtr src_corpus(new std::ifstream(src_file.c_str()));
    IfstreamPtr trg_corpus(new std::ifstream(trg_file.c_str()));
    IfstreamPtr alignments(new std::ifstream(aln_file.c_str()));
    
    OFFSET = vm["offset-sentences"].as<int>();
    LIMIT  = vm["limit-sentences"].as<int>();
    
    if(vm.count("resume") && vm.count("log")) {
        std::string fname = vm["log"].as<std::string>();
        std::ifstream log( fname.c_str() );
        
        std::string line = "0";
        while(std::getline(log, line)) {
            std::cerr << "Resuming from line " << line << std::endl;
            OFFSET = boost::lexical_cast<int>(line);    
        }
    }
    
    OfstreamPtr output;
    if(vm.count("output")) {
        std::string fname = vm["output"].as<std::string>();
        output.reset( new std::ofstream( fname.c_str() ) );
    }
        
    while(C < OFFSET) {
        std::string dummy;
        if(std::getline(*src_corpus, dummy) && std::getline(*trg_corpus, dummy) && std::getline(*alignments, dummy)) {
            C++;
        }
        else
            break;
        
        if(C % 100000 == 0)
            std::cerr << "Skipped " << C << " sentences" << std::endl; 
    }
    
    OfstreamPtr log;
    if(vm.count("log"))
        log.reset( new std::ofstream( vm["log"].as<std::string>().c_str() ) );    
    
    if(OFFSET) {
        if(log != false) {
            log->seekp(0);
            (*log) << C << std::endl;
        }
        std::cerr << "Skipped " << C << " sentences" << std::endl; 
    }
    
    int max_threads = vm["parallel"].as<int>();

    typedef boost::shared_ptr<boost::thread> ThreadPtr;
    std::vector<ThreadPtr> thread_vec;
    for(int i = 0; i < max_threads; i++) {
        Generator generator(vm, function_map);
    
        ThreadPtr t(new boost::thread(generator, src_corpus, trg_corpus, alignments, output, log));
        thread_vec.push_back(t);
    }
    for(int i = 0; i < max_threads; i++) {
        thread_vec[i]->join();
    }
}

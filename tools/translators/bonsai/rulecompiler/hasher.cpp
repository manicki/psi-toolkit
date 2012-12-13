#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "CompressedDAG.hpp"
#include "SimpleDAG.hpp"
#include "RuleSymbolNumberMap.hpp"
 
namespace po = boost::program_options;
 
 
int main(int argc, char* args[]) {
    po::options_description desc("Allowed options");
    
    desc.add_options()
	("help", "Display this message")
	("dag", po::value<std::string>(), "Read DAG from binary file arg (mandatory)")
	("sym", po::value<std::string>(), "Read Symbol Map from binary file arg")
        ("check", "Treat input as language strings , check if string belongs to DAG language (default)")
        ("hash", "Treat input as language strings , output hash numbers")
        ("unhash", "Treat input as hash numbers, output language strings")
        ("print", "Print DAG as text file")
        ("mode", po::value<int>(), "Memory mode")
    ;
			    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, args, desc), vm);
    po::notify(vm);
					
    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }	
    			
    poleng::bonsai::rules::CompressedDAG h;    
    if(vm.count("dag")) {
        if(vm.count("mode"))
            h.open(vm["dag"].as<std::string>(), vm["mode"].as<int>());
        else
            h.open(vm["dag"].as<std::string>(), 0);
    }
    else {
	std::cerr << "Error: No DAG file given." << std::endl;
	std::cout << desc << std::endl;
	return 1;
    }
    
    if(vm.count("print")) {
	h.print();
	return 1;
    }
    
    poleng::bonsai::rules::RuleSymbolNumberMap sym_map;
    if(vm.count("sym")) {
	sym_map.open(vm["sym"].as<std::string>());
    }
    
    if(vm.count("unhash")) {
        std::string line;
        while(getline(std::cin,line)) {
            try {
                poleng::bonsai::rules::Word w = h.unhash(boost::lexical_cast<int>(line));
                for(poleng::bonsai::rules::Word::iterator it = w.begin(); it != w.end(); it++) {
		    if(vm.count("sym"))
			std::cout << sym_map.get_label_by_number( *it );
		    else
			std::cout << *it;
			
                    poleng::bonsai::rules::Word::iterator tit = w.end();
                    if(it != --tit)
                        std::cout << " ";
                }
                std::cout << std::endl;
            }
            catch (...) {}
        }
    }
    else if(vm.count("check")) {
        std::string line;
        while(getline(std::cin,line)) {
            boost::regex re("(\\s+)");
            boost::sregex_token_iterator it(line.begin(), line.end(), re, -1);
            boost::sregex_token_iterator end;
            
            poleng::bonsai::rules::Word w;
            while(it != end) {
		if(vm.count("sym"))
		    w.push_back( sym_map.get_number_by_string(*it++) );
		else 
		    w.push_back(boost::lexical_cast<int>(*it++));
            }
            
	    std::cout << h.in(w) << std::endl;
        }
    }
    else {
        std::string line;
        while(getline(std::cin,line)) {
            boost::regex re("(\\s+)");
            boost::sregex_token_iterator it(line.begin(), line.end(), re, -1);
            boost::sregex_token_iterator end;
            
            poleng::bonsai::rules::Word w;
            while(it != end) {
		if(vm.count("sym"))
		    w.push_back( sym_map.get_number_by_string(*it++) );
		else 
		    w.push_back(boost::lexical_cast<int>(*it++));
            }
            
	    std::cout << h.hash(w) << std::endl;    
        }
    }
}


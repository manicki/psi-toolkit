#include <string>
#include <iostream>
#include <vector>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "MultiArray.hpp"
 
namespace po = boost::program_options;

int main(int argc, char* args[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
	("help", "Display this message")
	("out", po::value<std::string>(), "Filename for compiled version or memory map prefix (mandatory)")
        ("mapped", "Generate file for memory mapping")
        ("size", po::value<int>(), "Maximum size for memory mapped file")
    ;
	
    po::variables_map vm;
    po::store(po::parse_command_line(argc, args, desc), vm);
    po::notify(vm);
					
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }	
    	
    if(vm.count("out")) {
        poleng::bonsai::MultiArray* m;
        std::string file = vm["out"].as<std::string>();
        
        if(vm.count("mapped"))
            m = new poleng::bonsai::MappedMultiArray();
        else
            m = new poleng::bonsai::SerialMultiArray();
        
        int size = 0;
        if(vm.count("size"))
            size = vm["size"].as<int>();
        m->text2bin(std::cin, file, size);
    }
    else {
	std::cerr << "Error: No out file name given." << std::endl;
	std::cout << desc << std::endl;
	return 1;
    }
}

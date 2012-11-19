#include <boost/program_options.hpp>

#include "SortedArray.hpp"

namespace po = boost::program_options;

int main (int argc, char* args[]) {
    po::options_description desc("Allowed options");

    desc.add_options()
        ("help,h", "Display this message")
        ("infile,i", po::value<std::string>(), "Sorted input text file")
        ("outfile,o", po::value<std::string>(), "Output file name")
        ("binary,b", "Input file is binary data")
        ("text,t", "text")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, args, desc), vm);
    po::notify(vm);
      
    if (vm.count("help")) {
        std::cerr << desc << std::endl;
        return 1;
    }
    
    poleng::bonsai::rules::CharSortedArray csa;
    
    if(vm.count("text")) {
       csa.open(std::cin);
       csa.to_txt(std::cout);
       exit(0);
    }
    
    if(vm.count("infile")) {
        if(vm.count("binary")) {
            std::ifstream ifs(vm["infile"].as<std::string>().c_str(), std::ios::binary);
            csa.read_binary_data(ifs);   
        }
        else {
            std::ifstream ifs(vm["infile"].as<std::string>().c_str());
            csa.read_text_data(ifs);           
        }
    }
    else {
        if(vm.count("binary")) {
            csa.read_binary_data(std::cin);
        }
        else {
            csa.read_text_data(std::cin);
        }
    }
    
    std::cerr << csa.at(0) << std::endl;
    std::cerr << csa.at(100) << std::endl;
    std::cerr << csa.at(10000) << std::endl;
    std::cerr << csa.at(1000000) << std::endl;
    
    if(vm.count("outfile")) {
        std::ofstream ofs(vm["outfile"].as<std::string>().c_str(), std::ios::binary);
        csa.save(ofs);
    }
    else {
        csa.save(std::cout);
    }
    
    return 0;
}

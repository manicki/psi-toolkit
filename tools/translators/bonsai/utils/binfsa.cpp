#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include "CompressedDAGBuilder.hpp"

namespace po = boost::program_options;

int main (int argc, char* args[]) {
    po::options_description desc("Allowed options");

    desc.add_options()
        ("help", "Display this message")
        ("infile,i", po::value<std::string>(), "Sorted input text file (fsmlibrary format)")
        ("outfile,o", po::value<std::string>()->default_value("out"), "Prefix for states and arcs file")
        ("mode", po::value<int>()->default_value(0), "Memory mode for temporary DAG")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, args, desc), vm);
    po::notify(vm);
      
    if (vm.count("help")) {
        std::cerr << desc << std::endl;
        return 1;
    }
    
    poleng::bonsai::rules::CompressedDAGBuilder cdb;       
    if(vm.count("infile")) {
        std::ifstream ifs(vm["infile"].as<std::string>().c_str());
        cdb.compile(ifs, vm["outfile"].as<std::string>(), vm["mode"].as<int>());
    }
    else {
        cdb.compile(std::cin, vm["outfile"].as<std::string>(), vm["mode"].as<int>());
    }
    
    return 0;
}

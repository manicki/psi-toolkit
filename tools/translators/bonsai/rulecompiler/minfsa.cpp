#include <boost/program_options.hpp>

#include "IncrementalFSA.hpp"

poleng::bonsai::Word line_to_word(std::string line) {
    boost::regex re("(\\d+)");
    boost::sregex_token_iterator it(line.begin(), line.end(), re, 1);
    boost::sregex_token_iterator end;

    poleng::bonsai::Word w;
    while(it != end)
	w.push_back(boost::lexical_cast<int>(*it++));

    return w;
}

namespace po = boost::program_options;

int main (int argc, char* args[]) {
    po::options_description desc("Allowed options");

    desc.add_options()
        ("help", "Display this message")
        ("registers,r", po::value<int>()->default_value(50), "Number of registers kept in memory")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, args, desc), vm);
    po::notify(vm);
      
    if (vm.count("help")) {
        std::cerr << desc << std::endl;
        return 1;
    }
    
    poleng::bonsai::IncrementalFSA fsa(vm["registers"].as<int>());
    
    int c = 1;
    std::string line;
    while(getline(std::cin,line)) {
        poleng::bonsai::Word w = line_to_word(line);
	fsa.add_word(w);
	if(c % 100000 == 0)
	    std::cerr << "Added " << c << " words - actual size: " << fsa.size() << std::endl;
	c++;
    }
    fsa.finalize();
    
    std::cerr << "end" << std::endl;
    return 0;
}

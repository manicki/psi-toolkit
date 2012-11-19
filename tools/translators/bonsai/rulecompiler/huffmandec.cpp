#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <rules/Huffman.hpp>
#include <rules/RuleSymbolNumberMap.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#define SMALL_INT_BYTES 3

namespace po = boost::program_options;

int main(int argc, char* args[]) {
    po::options_description desc("Allowed options");
    
    desc.add_options()
	("help", "Display this message")
	("in", po::value<std::string>(), "Path to huffed file")
	("syms", po::value<std::string>(), "Path to symbol file")
    ;
			    
    po::variables_map cli;
    po::store(po::parse_command_line(argc, args, desc), cli);
    po::notify(cli);

    if (cli.count("help")) {
        std::cerr << desc << std::endl;
        return 1;
    }

    if (!cli.count("in")) {
        std::cerr << desc << std::endl;
        return 1;
    }

    std::ifstream in;
    in.open(cli["in"].as<std::string>().c_str(), std::ios::binary);
    
    poleng::bonsai::Huffman hw(SMALL_INT_BYTES);
    hw.read_code_table_from_stream(in);
    //hw.generate_code_table_from_file(cli["in"].as<std::string>());
    
    poleng::bonsai::rules::RuleSymbolNumberMap src_sym_map;
    src_sym_map.open(cli["syms"].as<std::string>());
    
    boost::posix_time::ptime pt_start = boost::posix_time::microsec_clock::local_time();
    
    std::string line;
    while(std::getline(std::cin, line)) {
        int bytepos = boost::lexical_cast<int>(line);
	
	in.seekg(bytepos);
	
	short unsigned int huff_length;
	in.read((char*)&huff_length, sizeof(huff_length));
	
	unsigned char* inbuffer = new unsigned char[huff_length];
	unsigned int inlength = huff_length;
	
	unsigned char* outbuffer = NULL;
	unsigned int outlength = 0;
	
	in.read((char*)inbuffer, inlength);
	hw.decode_string(inbuffer, inlength, outbuffer, outlength);
		
	int i = 0;
	int step = SMALL_INT_BYTES;
	
	while(i < outlength) {
		unsigned char syms = (unsigned char)outbuffer[i];
		i++;
		
		int j = 0;
		while(j < syms) {
			unsigned int s = 0;
			memcpy(&s, outbuffer + i, step);
			
			i += step;
			j++;
			
			std::cerr <<  src_sym_map.get_label_by_number(s) << " ";
		}
		std::cerr << std::endl;
	}
	
	delete [] inbuffer;
	free(outbuffer);
	
    }
    
    boost::posix_time::ptime pt_end = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta = pt_end - pt_start;
    int d = delta.total_milliseconds();
    
    std::cerr << "Total: " << d << std::endl;
    
    return 0;
}

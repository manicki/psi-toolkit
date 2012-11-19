#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "Huffman.hpp"

namespace po = boost::program_options;

typedef std::vector<int> Word;

int main(int argc, char* args[]) {
        po::options_description desc("Allowed options");
    
    desc.add_options()
	("help", "Display this message")
	("in_src", po::value<std::string>(), "Path to indexed source language rule part")
	("in_trg", po::value<std::string>(), "Path to packed target language rule part")
	("out_src", po::value<std::string>(), "Path to save new source langue rule part")
	("out_trg", po::value<std::string>(), "Path to save new huffed target language rule part")
    ;
			    
    po::variables_map cli;
    po::store(po::parse_command_line(argc, args, desc), cli);
    po::notify(cli);

    if (cli.count("help")) {
        std::cerr << desc << std::endl;
        return 1;
    }

    if (!cli.count("in_src") or !cli.count("in_trg") or !cli.count("out_src") or !cli.count("out_trg")) {
        std::cerr << desc << std::endl;
        return 1;
    }

    poleng::bonsai::Huffman hw;
    
    std::string insrcfile = cli["in_src"].as<std::string>();
    std::string intrgfile = cli["in_trg"].as<std::string>();
    std::string outsrcfile = cli["out_src"].as<std::string>();
    std::string outtrgfile = cli["out_trg"].as<std::string>();
    
    hw.generate_code_table_from_file(intrgfile.c_str());
    
    std::ifstream insrc;
    insrc.open(insrcfile.c_str(), std::ios::binary);
    
    std::ifstream intrg;
    intrg.open(intrgfile.c_str(), std::ios::binary);
    
    std::ofstream outsrc;
    outsrc.open(outsrcfile.c_str(), std::ios::binary);
    
    std::ofstream outtrg;
    outtrg.open(outtrgfile.c_str(), std::ios::binary);
    
    hw.write_code_table_to_stream(outtrg);
    
    std::string line;
    int c = 1;
    while(std::getline(insrc, line)) {
	int act_pos = boost::lexical_cast<int>(line);
	intrg.seekg(act_pos);
        
	unsigned int inlength;
	intrg.read((char*)&inlength, sizeof(unsigned int));
	
        unsigned char* inbuffer = new unsigned char[inlength];
        
        intrg.read((char*)inbuffer, inlength);
	
	unsigned int outlength = 0;
        unsigned char* outbuffer = NULL;
        
        hw.encode_string(inbuffer, inlength, outbuffer, outlength);
        
	unsigned short int huff_length = outlength;
	
        int huff_pos = outtrg.tellp();
        //outtrg.write((char*)&huff_length, sizeof(short int));
        outtrg.write((char*)outbuffer, huff_length);
        
	delete [] inbuffer;
	free(outbuffer);
        	
	outsrc << huff_pos << std::endl;
        
        //last_pos = act_pos;
	
	if(c % 100000 == 0)
	   std::cerr << "Huffed " << c << " target rule sets" << std::endl;
	c++;
    }
    return 0;
}

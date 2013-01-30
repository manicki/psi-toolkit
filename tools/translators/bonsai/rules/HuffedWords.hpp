#ifndef HUFFEDWORDS_HPP__
#define HUFFEDWORDS_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "StringVector.hpp"
#include "RuleTypes.hpp"

namespace poleng
{

namespace bonsai
{

namespace rules {
    
    class HuffedWords {
      private:
	StringVector<> words;
				    
      public:
        HuffedWords();
        
        void open(std::string); // read the huffman tree and keep it in memory (optionally: read first n entries into memory)
        WordList get_words(unsigned int);
        WordTriples get_wordtriples(unsigned int);
    };
    
}

}

}

#endif

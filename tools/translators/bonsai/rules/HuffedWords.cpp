#include "HuffedWords.hpp"
#include <sstream>

namespace poleng
{
    
namespace bonsai
{

using namespace rules;

HuffedWords::HuffedWords()  {}
 
void HuffedWords::open(std::string filename) {
    words.load(filename);
}

WordTriples HuffedWords::get_wordtriples(unsigned int hash_no) {
    WordTriples wordtriples;   
    std::stringstream ss(words[hash_no]);
    
    unsigned char l, m, n;
    unsigned int j;
    unsigned char a;
    
    while(ss >> l >> m >> n) {
        Word word;
        Word probs;
        Word alignment;
        
        for(int i = 0; i < l; i++) {
            ss.read((char*)&j, 4);
            word.push_back(j);
        }
        for(int i = 0; i < m; i++) {
            ss.read((char*)&j, 4);
            probs.push_back(j);
        }
        for(int i = 0; i < n; i++) {
            ss.read((char*)&a, 1);
            alignment.push_back(a);
        }
        
        wordtriples.push_back(WordTriple(word, probs, alignment));
    }
    return wordtriples;    
}

}

}

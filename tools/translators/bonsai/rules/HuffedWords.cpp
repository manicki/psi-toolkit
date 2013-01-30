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


WordList HuffedWords::get_words(unsigned int hash_no) {
    WordList words;
    //huffed_stream.seekg(hash_no);
    //
    //unsigned int inlength = length; 
    //unsigned char* inbuffer = new unsigned char[inlength];
    //
    //unsigned char* outbuffer = NULL;
    //unsigned int outlength = 0;
    //
    //huffed_stream.read((char*)inbuffer, inlength);
    //h.decode_string(inbuffer, inlength, outbuffer, outlength);
    //        
    //int i = 0;
    //while(i < outlength) {
    //    Word w;
    //    
    //    unsigned char symbols = (unsigned char)outbuffer[i];
    //    i++;
    //    
    //    int j = 0;
    //    while(j < symbols) {
    //        unsigned int s = 0;
    //        memcpy(&s, outbuffer + i, step);
    //        
    //        i += step;
    //        j++;
    //        
    //        w.push_back(s);
    //    }
    //    words.push_back(WordIndex(w, hash_no));
    //}
    //
    //delete [] inbuffer;
    //free(outbuffer);
    
    return words;
}

}

}

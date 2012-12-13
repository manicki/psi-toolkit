#include "HuffedWords.hpp"

namespace poleng
{
    
namespace bonsai
{

using namespace rules;

HuffedWords::HuffedWords(int step_) : h(Huffman(step_)), step(step_) {}
HuffedWords::HuffedWords() : h(Huffman(3)), step(3) {}
 
void HuffedWords::open(std::string filename) {
    
    /* Open compressed file for reading */
    huffed_stream.open(filename.c_str(), std::ios::binary);
    
    /* Read huffman tree from file beginning */
    h.read_code_table_from_stream(huffed_stream);    
}

WordTriples HuffedWords::get_wordtriples(unsigned int hash_no, unsigned int length) {
    
    WordTriples wordtriples;
    
    huffed_stream.seekg(hash_no);
    
    unsigned int inlength = length; 
    unsigned char* inbuffer = new unsigned char[inlength];
    
    unsigned char* outbuffer = NULL;
    unsigned int outlength = 0;
    
    huffed_stream.read((char*)inbuffer, inlength);
    h.decode_string(inbuffer, inlength, outbuffer, outlength);
            
    int i = 0;
    while(i < outlength) {
        Word word;
        Word probs;
        Word alignment;
        
        unsigned char word_symbols = (unsigned char)outbuffer[i++];
        unsigned char prob_symbols = (unsigned char)outbuffer[i++];
        unsigned char algn_symbols = (unsigned char)outbuffer[i++];
                
        int j = 0;
        while(j < word_symbols) {
            unsigned int s = 0;
            memcpy(&s, outbuffer + i, step);
            
            i += step;
            j++;
            
            word.push_back(s);
        }
        j = 0;
        while(j < prob_symbols) {
            unsigned int s = 0;
            memcpy(&s, outbuffer + i, step);
            
            i += step;
            j++;
            
            probs.push_back(s);
        }
        j = 0;
        while(j < algn_symbols) {
            unsigned int s = 0;
            memcpy(&s, outbuffer + i, 1);
            
            i++;
            j++;
            
            alignment.push_back(s);
        }
        wordtriples.push_back(WordTriple(word, probs, alignment));
    }
    
    delete [] inbuffer;
    free(outbuffer);
    
    return wordtriples;    
}


WordList HuffedWords::get_words(unsigned int hash_no, unsigned int length) {
    WordList words;
    huffed_stream.seekg(hash_no);
    
    unsigned int inlength = length; 
    unsigned char* inbuffer = new unsigned char[inlength];
    
    unsigned char* outbuffer = NULL;
    unsigned int outlength = 0;
    
    huffed_stream.read((char*)inbuffer, inlength);
    h.decode_string(inbuffer, inlength, outbuffer, outlength);
            
    int i = 0;
    while(i < outlength) {
        Word w;
        
        unsigned char symbols = (unsigned char)outbuffer[i];
        i++;
        
        int j = 0;
        while(j < symbols) {
            unsigned int s = 0;
            memcpy(&s, outbuffer + i, step);
            
            i += step;
            j++;
            
            w.push_back(s);
        }
        words.push_back(WordIndex(w, hash_no));
    }
    
    delete [] inbuffer;
    free(outbuffer);
    
    return words;
}

}

}

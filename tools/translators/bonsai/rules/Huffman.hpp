#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <cstring>

#ifdef WIN32
#include <winsock2.h>
#include <malloc.h>
#define alloca _alloca
#else
#include <netinet/in.h>
#endif

#include <string>

#define CACHE_SIZE 1024	
#define MAX_SYMBOLS 256

namespace poleng
{

namespace bonsai
{

class Huffman {
  public:
    Huffman() : has_code_table(false), symbol_count(0), se(NULL), root(NULL), step(3) {
	init_frequencies(&sf);
    }
    Huffman(int step_) : has_code_table(false), symbol_count(0), se(NULL), root(NULL), step(step_) {
	init_frequencies(&sf);
    }
    ~Huffman() {
	if(se != NULL and root != NULL) {
	  free_huffman_tree(root);
	  free_encoder(se);
	}
    }
    
    void update_symbol_frequencies(unsigned char* buffer, unsigned int length) {
	for(int i = 0; i < length; i++) {
		unsigned char uc = buffer[i];
		if(!sf[uc])
			sf[uc] = new_leaf_node(uc);
		++sf[uc]->count;
	}
    }
    
    void generate_code_table_from_sf() {
	  print_freqs(&sf);
	  se = calculate_huffman_codes(&sf);
	  root = sf[0];
	  has_code_table = true;
    }
    
    void generate_code_table_from_file(std::string filename)  {
        FILE* in = fopen(filename.c_str() , "r");
        if(in != NULL) {
            symbol_count = get_symbol_frequencies(&sf, in);
	    print_freqs(&sf);
	    std::cerr << "Sym count: " << symbol_count << std::endl;
            se = calculate_huffman_codes(&sf);
	    root = sf[0];
            has_code_table = true;
        }
    }
    
    template<class OStream>
    void write_code_table_to_stream(OStream &out) {
	unsigned long i, count = 0;
	
	for(i = 0; i < MAX_SYMBOLS; ++i) {
		if((*se)[i])
			++count;
	}

	i = htonl(count);
	out.write((char*)&i, sizeof(i) * 1);
	
	symbol_count = htonl(symbol_count);
	out.write((char*)&symbol_count, sizeof(symbol_count)*1);
	
	for(i = 0; i < MAX_SYMBOLS; ++i) {
		huffman_code *p = (*se)[i];
		if(p) {
			unsigned int numbytes;
			out.put((unsigned char)i);
			out.put(p->numbits);
			numbytes = numbytes_from_numbits(p->numbits);
			out.write((char*)p->bits, 1 * numbytes);
		}
	}
    }
    
    template <class IStream>
    unsigned int read_code_table_from_stream(IStream &in) {
	unsigned int what_ever;
	unsigned int *pDataBytes = &what_ever;
	root = new_nonleaf_node(0, NULL, NULL);
	unsigned int count;
	
	in.read((char*)&count, sizeof(count)*1);
	count = ntohl(count);

	in.read((char*)pDataBytes, sizeof(*pDataBytes)*1);
	*pDataBytes = ntohl(*pDataBytes);

	while(count-- > 0) {
		char c;
		unsigned int curbit;
		unsigned char symbol;
		unsigned char numbits;
		unsigned char numbytes;
		unsigned char *bytes;
		huffman_node *p = root;
		
		in.get(c);
		symbol = (unsigned char)c;
		
		in.get(c);
		numbits = (unsigned char)c;
		
		numbytes = (unsigned char)numbytes_from_numbits(numbits);
		bytes = (unsigned char*)malloc(numbytes);
		in.read((char*)bytes, 1*numbytes);
		
		for(curbit = 0; curbit < numbits; ++curbit) {
			if(get_bit(bytes, curbit)) {
				if(p->one == NULL) {
					p->one = curbit == (unsigned char)(numbits - 1)
						? new_leaf_node(symbol)
						: new_nonleaf_node(0, NULL, NULL);
					p->one->parent = p;
				}
				p = p->one;
			}
			else {
				if(p->zero == NULL) {
					p->zero = curbit == (unsigned char)(numbits - 1)
						? new_leaf_node(symbol)
						: new_nonleaf_node(0, NULL, NULL);
					p->zero->parent = p;
				}
				p = p->zero;
			}
		}
		
		free(bytes);
	}
	has_code_table = true;
	return in.tellg();
    }
    
    
    std::string encode_string(std::string s) {
	unsigned char* buf = NULL;
	unsigned int buflen = 0;
	
	encode_string((unsigned char*)s.c_str(), s.size(), buf, buflen);
	std::string result((char*)buf, buflen);
	free(buf);
	return result;
    }
    
    void encode_string(unsigned char *in, unsigned int inlen, unsigned char *&out, unsigned int &outlen) {
        if(has_code_table) {
            
	    unsigned int bufinlen = inlen+sizeof(unsigned int);
            unsigned char *bufin = (unsigned char*)malloc(bufinlen);
	    
	    std::memcpy((char*)bufin, (char*)&inlen, sizeof(unsigned int));
	    unsigned char *bufin_shifted = bufin+4;
	    
	    std::memcpy((char*)bufin_shifted, (char*)in, inlen);
            
            buf_cache cache;
            init_cache(&cache, CACHE_SIZE, &out, &outlen);
    
            do_memory_encode(&cache, bufin, bufinlen, se);
            flush_cache(&cache);
            free_cache(&cache);
        }
    }
	
    std::string decode_string(std::string s) {
	unsigned char* buf = NULL;
	unsigned int buflen = 0;
	decode_string((unsigned char*)s.c_str(), s.length(), buf, buflen);
	std::string result((char*)buf, buflen);
	free(buf);
	return result;
    }
    
    void decode_string(unsigned char* in, unsigned int inlen, unsigned char *&out, unsigned int &outlen) {
        if(has_code_table) {
            
            unsigned char *bufin = in;
            unsigned int bufinlen = inlen;
            
	    huffman_node *p;
	    unsigned int i = 0;
	    unsigned char *buf;
	    unsigned int bufcur = 0;
	    
	    unsigned int counter = 0;
	    unsigned char* counterbuf = (unsigned char*)&counter;
	    
	    buf = (unsigned char*)malloc(bufcur+1);
    
	    p = root;
	    for(; i < bufinlen; ++i) {
		    unsigned char byte = bufin[i];
		    unsigned char mask = 1;
		    
		    while(mask) {
			    p = byte & mask ? p->one : p->zero;
			    mask <<= 1;
			    
			    if(p->isLeaf) {
				    if(bufcur < sizeof(unsigned int)) {
					counterbuf[bufcur++] = p->symbol;
				    }
				    else {
					buf = (unsigned char*)realloc(buf,bufcur+1-sizeof(counter));
					buf[bufcur++-sizeof(counter)] = p->symbol;
				      
					if(counter > 0)
					    counter--;
				    }
				    
				    p = root;
				    
				    if(i==bufinlen-1 and counter == 0)
					break;
				
			    }
		    }
	    }
    
	    outlen = bufcur-sizeof(unsigned int);
	    out = buf;
        }
    }
	
  private:
    unsigned int symbol_count;
    bool has_code_table;
    int step;

    typedef struct huffman_node_tag
    {
	    unsigned char isLeaf;
	    unsigned long count;
	    struct huffman_node_tag *parent;
    
	    union
	    {
		    struct
		    {
			    struct huffman_node_tag *zero, *one;
		    };
		    unsigned char symbol;
	    };
    } huffman_node;
    
    typedef struct huffman_code_tag
    {
	    unsigned long numbits;
	    unsigned char *bits;
    } huffman_code;
    
    typedef struct buf_cache_tag
    {
	    unsigned char *cache;
	    unsigned int cache_len;
	    unsigned int cache_cur;
	    unsigned char **pbufout;
	    unsigned int *pbufoutlen;
    } buf_cache;
    
    typedef huffman_node* SymbolFrequencies[MAX_SYMBOLS];
    typedef huffman_code* SymbolEncoder[MAX_SYMBOLS];

    huffman_node *root;
    SymbolEncoder *se;
    SymbolFrequencies sf;
    
    unsigned long numbytes_from_numbits(unsigned long numbits);
    unsigned char get_bit(unsigned char* bits, unsigned long i);
    void reverse_bits(unsigned char* bits, unsigned long numbits);
    huffman_code* new_code(const huffman_node* leaf);
    huffman_node* new_leaf_node(unsigned char symbol);
    huffman_node* new_nonleaf_node(unsigned long count, huffman_node *zero, huffman_node *one);
    void free_huffman_tree(huffman_node *subtree);
    void free_code(huffman_code* p);
    void free_encoder(SymbolEncoder *pSE);
    void init_frequencies(SymbolFrequencies *pSF);
    int init_cache(buf_cache* pc, unsigned int cache_size, unsigned char **pbufout, unsigned int *pbufoutlen);
    void free_cache(buf_cache* pc);
    int flush_cache(buf_cache* pc);
    int write_cache(buf_cache* pc, const void *to_write, unsigned int to_write_len);
    unsigned int get_symbol_frequencies(SymbolFrequencies *pSF, FILE *in);
    unsigned int get_symbol_frequencies_from_memory(SymbolFrequencies *pSF, const unsigned char *bufin, unsigned int bufinlen);
    
    void print_freqs(SymbolFrequencies * pSF);
    
    void build_symbol_encoder(huffman_node *subtree, SymbolEncoder *pSF);
    SymbolEncoder* calculate_huffman_codes(SymbolFrequencies * pSF);
    
    int write_code_table_to_memory(buf_cache *pc, SymbolEncoder *se, unsigned int symbol_count);
    int memread(const unsigned char* buf, unsigned int buflen, unsigned int *pindex, void* bufout, unsigned int readlen);
    huffman_node* read_code_table_from_memory(const unsigned char* bufin, unsigned int bufinlen, unsigned int *pindex, unsigned int *pDataBytes);
    int do_memory_encode(buf_cache *pc, const unsigned char* bufin, unsigned int bufinlen, SymbolEncoder *se);
    
    static int SFComp(const void *p1, const void *p2) {
	const huffman_node *hn1 = *(const huffman_node**)p1;
	const huffman_node *hn2 = *(const huffman_node**)p2;

	/* Sort all NULLs to the end. */
	if(hn1 == NULL && hn2 == NULL)
		return 0;
	if(hn1 == NULL)
		return 1;
	if(hn2 == NULL)
		return -1;
	
	if(hn1->count > hn2->count)
		return 1;
	else if(hn1->count < hn2->count)
		return -1;

	return 0;
    }

};	

}

}

#endif

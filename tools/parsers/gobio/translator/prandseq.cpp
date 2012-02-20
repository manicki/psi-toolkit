#include "prandseq.h"
#include <math.h>

unsigned int prandomSequence(unsigned int x)
{
    return x+1;
}

unsigned int getPrandomStartKey() 
{
    return 8042003;
}

void doxor(char* text, unsigned int len, unsigned int* key) 
{
    while( len>0 ) {
        *key = prandomSequence(*key);
        *text = (unsigned char)(*text ^ *key);
        ++text;
        --len;
    }
}

void imoaxor(char* text, unsigned int len, imoa_key& key) 
{
    while( len>0 ) {
        int k = imoa_irandom(key,0,256);
        *text = (unsigned char)(*text ^ k);
        ++text;
        --len;
    }
}

void imoaxor(std::string& text, imoa_key& key)
{
    size_t len = text.length();
    size_t i = 0;
    while( len>0 ) {
        int k = imoa_irandom(key,0,256);
        text[i] = (unsigned char)(text[i] ^ k);
        ++i;
        --len;
    }
}

prand_uint32 imoa_random(imoa_key& key) 
{
    prand_uint64 _2_32 = ((prand_uint64)65536) * 65536;
    prand_uint64 s;

    s = ((prand_uint64)2111111111) * key.x_4;
    s+= 1492 * (key.x_4 = key.x_3);
    s+= 1776 * (key.x_3 = key.x_2);
    s+= 5115 * (key.x_2 = key.x_1);
    s+= key.c;

    key.x_1 = s%_2_32;
    key.c = s/_2_32;
    return key.x_1;
}

int imoa_irandom(imoa_key& key, int min, int max) {
  int iinterval = max - min + 1;
  if (iinterval <= 0) return 0x80000000; // error
  int i = imoa_random(key)%iinterval;     // truncate
  if (i >= iinterval) i = iinterval-1;
  return min + i;
}

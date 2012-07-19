#ifndef prandseqH
#define prandseqH

#include <string>

#if (defined __GNUG__)
typedef unsigned int prand_uint32;    
typedef unsigned long long prand_uint64;     
#elif (defined __BORLANDC__)
typedef unsigned __int32 prand_uint32;     
typedef unsigned __int64 prand_uint64;     
#elif (defined _T5_OS_WINDOWS)
typedef unsigned __int32 prand_uint32;     
typedef unsigned __int64 prand_uint64;     
#else
#error Jak u Ciebie okre¶la siê integera 32 i 64 bitowego?
#endif

/**
 * Klucz Mother-of-all
 */ 
struct imoa_key {
    prand_uint32 x_4;
    prand_uint32 x_3;
    prand_uint32 x_2;
    prand_uint32 x_1;
    prand_uint32 c;

    void set( prand_uint32 a1
            , prand_uint32 a2
            , prand_uint32 a3
            , prand_uint32 a4
            , prand_uint32 a5 )
    {
      x_4 = a1;
      x_3 = a2;
      x_2 = a3;
      x_1 = a4;
      c = a5;
    }
  };


/**
 * Inicjacja Mother-of-all
 */ 
void imoa_init(imoa_key& key, prand_uint32 seed);

/**
 * Daje losowy int z zadanego przedzialu.
 */
int imoa_irandom(imoa_key& key, int min, int max);

/**
 * Funkcja generuje pseudolosow± sekwencjê - przydatne do XOR-owania
 * @param x poprzednia warto¶æ w sekwencji
 * @returns nastêpna warto¶æ w sekwencji
 */
unsigned int prandomSequence(unsigned int x);

/**
 * Funkcja generuje pocz±tkowy klucz dla prandomSequence.
 * @todo Podawaæ jaki¶ parametr? (np. nazwê pliku, ¿eby nie by³o, ¿e wszystkie
 *       kodowane s± tak samo)
 */ 
unsigned int getPrandomStartKey();

/**
 * XORuje ci±g.
 * Przed pierwszym wywo³aniem nale¿y wygenerowaæ sobie 
 * klucz (np. getPrandomStartKey)
 *
 * @param text Napis do przeXORowania.
 * @param len  Ilo¶æ znaków do przeXORowania.
 * @param key  Ostatnio u¿yty klucz.
 */ 
void doxor(char* text, unsigned int len, unsigned int* key);

/**
 * XORuje ci±g (podobnie jak doxor(...) ) ale korzysta 
 * z generatora Mother-of-all.
 *
 * @see doxor(char*, uint, uint)
 */ 
void imoaxor(char* text, unsigned int len, imoa_key& key);

void imoaxor(std::string& text, imoa_key& key);

#endif

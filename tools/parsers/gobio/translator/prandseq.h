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
#error Jak u Ciebie okresla sie integera 32 i 64 bitowego?
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

    void set(
        prand_uint32 a1,
        prand_uint32 a2,
        prand_uint32 a3,
        prand_uint32 a4,
        prand_uint32 a5
    ) {
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
 * Funkcja generuje pseudolosowa sekwencje - przydatne do XOR-owania
 * @param x poprzednia wartosc w sekwencji
 * @returns nastepna wartosc w sekwencji
 */
unsigned int prandomSequence(unsigned int x);

/**
 * Funkcja generuje poczatkowy klucz dla prandomSequence.
 * @todo Podawac jakis parametr? (np. nazwe pliku, zeby nie bylo, ze wszystkie
 *       kodowane sa tak samo)
 */
unsigned int getPrandomStartKey();

/**
 * XORuje ciag.
 * Przed pierwszym wywolaniem nalezy wygenerowac sobie
 * klucz (np. getPrandomStartKey)
 *
 * @param text Napis do przeXORowania.
 * @param len  Ilosc znakow do przeXORowania.
 * @param key  Ostatnio uzyty klucz.
 */
void doxor(char* text, unsigned int len, unsigned int* key);

/**
 * XORuje ciag (podobnie jak doxor(...) ) ale korzysta
 * z generatora Mother-of-all.
 *
 * @see doxor(char*, uint, uint)
 */
void imoaxor(char* text, unsigned int len, imoa_key& key);

void imoaxor(std::string& text, imoa_key& key);

#endif

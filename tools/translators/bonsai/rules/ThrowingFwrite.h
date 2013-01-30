#ifndef moses_ThrowingFwrite_h
#define moses_ThrowingFwrite_h

#include <cassert>
#include <cstdio>

size_t ThrowingFwrite(const void *ptr, size_t size, size_t count, FILE* stream);

#endif

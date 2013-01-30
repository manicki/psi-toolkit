#include "ThrowingFwrite.h" 

size_t ThrowingFwrite(const void *ptr, size_t size, size_t count, FILE* stream) {
  assert(size);
  size_t returnValue = std::fwrite(ptr, size, count, stream);
  //UTIL_THROW_IF(count != returnValue, util::ErrnoException, "Short fwrite; requested size " << size);
  return returnValue;
}

#ifndef LOGGING_HDR
#define LOGGING_HDR

#include <iostream>

#define TRACE(M) \
    do { \
        std::cerr << M << std::endl; \
    } while(0)

#define DEBUG(M) \
    do { \
      std::cerr << M << std::endl; \
    } while(0)

#define INFO(M) \
    do { \
      std::cerr << M << std::endl; \
    } while(0)

#define WARN(M) \
    do { \
      std::cerr << M << std::endl; \
    } while(0)

#define ERROR(M) \
    do { \
      std::cerr << M << std::endl; \
    } while(0)

#define FATAL(M) \
    do { \
      std::cerr << M << std::endl; \
    } while(0)

#endif

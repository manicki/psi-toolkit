#ifndef LOGGING_HDR
#define LOGGING_HDR

#include "config.hpp"

#if HAVE_LOG4CPP

#include <iostream>
#include <sstream>

#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/SimpleLayout.hh>

class PSILogger {
public:
    PSILogger();

    void trace(const std::string & msg);
    void debug(const std::string & msg);
    void info(const std::string & msg);
    void warn(const std::string & msg);
    void error(const std::string & msg);
    void fatal(const std::string & msg);

    void flush(log4cpp::Priority::PriorityLevel priorityLevel);

    PSILogger & operator<< (const std::string & msg);
    PSILogger & operator<< (const char * msg);
    PSILogger & operator<< (unsigned long msg);
    PSILogger & operator<< (signed long msg);
    PSILogger & operator<< (unsigned int msg);
    PSILogger & operator<< (signed int msg);
    PSILogger & operator<< (unsigned short msg);
    PSILogger & operator<< (signed short msg);
    PSILogger & operator<< (float msg);
    PSILogger & operator<< (double msg);
    PSILogger & operator<< (bool msg);
    
private:
    void initialize_logger_();

    std::stringstream buffer;
    log4cpp::Category & logger_category;
};

extern PSILogger psi_logger;

#define TRACE(M) \
    do { \
        psi_logger << M;                        \
        psi_logger.flush(log4cpp::Priority::DEBUG);  \
    } while (0)

#define DEBUG(M) \
    do { \
        psi_logger << M;                        \
        psi_logger.flush(log4cpp::Priority::DEBUG);                     \
    } while (0)

#define INFO(M) \
    do { \
        psi_logger << M;                        \
        psi_logger.flush(log4cpp::Priority::INFO);                     \
    } while (0)

#define WARN(M) \
    do { \
        psi_logger << M;                        \
        psi_logger.flush(log4cpp::Priority::WARN);                     \
    } while (0)

#define ERROR(M) \
    do { \
        psi_logger << M;                        \
        psi_logger.flush(log4cpp::Priority::ERROR);    \
    } while (0)

#define FATAL(M) \
    do { \
        psi_logger << M;                        \
        psi_logger.flush(log4cpp::Priority::FATAL);                     \
    } while (0)

#else

#include <iostream>


#define TRACE(M) \
    do { \
        std::cerr << M << std::endl; \
    } while (0)

#define DEBUG(M) \
    do { \
      std::cerr << M << std::endl; \
    } while (0)

#define INFO(M) \
    do { \
      std::cerr << M << std::endl; \
    } while (0)

#define WARN(M) \
    do { \
      std::cerr << M << std::endl; \
    } while (0)

#define ERROR(M) \
    do { \
      std::cerr << M << std::endl; \
    } while (0)

#define FATAL(M) \
    do { \
      std::cerr << M << std::endl; \
    } while (0)

#endif

#endif

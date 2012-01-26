#ifndef LOGGING_HDR
#define LOGGING_HDR

#include "config.hpp"

#if HAVE_LOG4CPP

#include <iostream>
#include <sstream>

#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/SimpleLayout.hh>
#include <log4cpp/Priority.hh>


class PSILogger {
public:
    PSILogger();
    ~PSILogger();

    void setLoggingToFile(const std::string & filepath);
    void setLoggingPriority(const std::string & priorityName);

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
    void setDefaultLoggerAppender_();
    void addDefaultLayoutToAppender_(log4cpp::Appender * appender);
    void setNewLoggerAppender_(log4cpp::Appender * appender);

    std::stringstream buffer;
    log4cpp::Category & logger_category;
    log4cpp::Appender * current_logger_appender;
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


#define SET_LOGGER_FILE(M) do { psi_logger.setLoggingToFile(M); } while(0);
#define SET_LOGGING_LEVEL(M) do { psi_logger.setLoggingPriority(M); } while(0);

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

#define SET_LOGGER_FILE(M) do { INFO("Logging to file unsupported (no log4cpp)."); } while(0);
#define SET_LOGGING_LEVEL(M) do { INFO("Logging priorities unsupported (no log4cpp)."); } while(0);

#endif

#endif

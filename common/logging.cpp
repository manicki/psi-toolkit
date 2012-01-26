#include "logging.hpp"

#if HAVE_LOG4CPP

PSILogger psi_logger;

PSILogger::PSILogger() : logger_category(log4cpp::Category::getInstance("Category")) {
    initialize_logger_();
}

void PSILogger::initialize_logger_() {
    /*Setting up Appender, layout and Category*/
    log4cpp::Appender *appender =
        new log4cpp::OstreamAppender(
                                     "OstreamAppender",&std::cout);

    log4cpp::Layout *layout =
        new log4cpp::SimpleLayout();

    appender->setLayout(layout);
    logger_category.setAppender(appender);
    logger_category.setPriority(log4cpp::Priority::INFO);
}


void PSILogger::trace(const std::string & msg) {
    std::cerr << "trace: " << msg << std::endl;
}

void PSILogger::debug(const std::string & msg) {
    std::cerr << "debug: " << msg << std::endl;
}

void PSILogger::info(const std::string & msg) {
    std::cerr << "info: " << msg << std::endl;
}

void PSILogger::warn(const std::string & msg) {
    std::cerr << "warn: " << msg << std::endl;
}

void PSILogger::error(const std::string & msg) {
    std::cerr << "error: " << msg << std::endl;
}

void PSILogger::fatal(const std::string & msg) {
    std::cerr << "fatal: " << msg << std::endl;
}

void PSILogger::flush(log4cpp::Priority::PriorityLevel priorityLevel) {
    logger_category.info(buffer.str());
    buffer.str("");
}

PSILogger & PSILogger::operator<< (const std::string & msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (const char * msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (unsigned long msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (signed long msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (unsigned int msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (signed int msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (unsigned short msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (signed short msg) {
    buffer << msg;
    return *this;
}
    

PSILogger & PSILogger::operator<< (float msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (double msg) {
    buffer << msg;
    return *this;
}

PSILogger & PSILogger::operator<< (bool msg) {
    buffer << msg;
    return *this;
}

#endif

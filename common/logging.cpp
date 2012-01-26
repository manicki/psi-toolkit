#include "logging.hpp"

#if HAVE_LOG4CPP

#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>

PSILogger psi_logger;

PSILogger::PSILogger() : logger_category(log4cpp::Category::getInstance("Category")) {
    initialize_logger_();
}

void PSILogger::initialize_logger_() {
    setDefaultLoggerAppender_();
    logger_category.setPriority(log4cpp::Priority::INFO);
}

void PSILogger::setDefaultLoggerAppender_() {
    log4cpp::Appender * default_logger_appender =
        new log4cpp::OstreamAppender(
                                     "OstreamAppender", &std::cerr);

    addDefaultLayoutToAppender_(default_logger_appender);

    setNewLoggerAppender_(default_logger_appender);
}

void PSILogger::addDefaultLayoutToAppender_(log4cpp::Appender * appender) {
    log4cpp::PatternLayout * layout =
        new log4cpp::PatternLayout();

    layout->setConversionPattern("%p %d{%Y-%m-%d %H:%M:%S,%l} : %m%n");

    appender->setLayout(layout);
}

PSILogger::~PSILogger() {
}

void PSILogger::setLoggingToFile(const std::string & filepath) {
    log4cpp::Appender * appender =
        new log4cpp::FileAppender("FileAppender", filepath.c_str());

    setNewLoggerAppender_(appender);
}

void PSILogger::setLoggingPriority(const std::string & priorityName) {
    try {
        log4cpp::Priority::Value newPriority =
            log4cpp::Priority::getPriorityValue(priorityName);
        logger_category.setPriority(newPriority);

    } catch (std::invalid_argument e) {
        ERROR("Unknown priority name: " << priorityName);
    }
}

void PSILogger::setNewLoggerAppender_(log4cpp::Appender * appender) {
    logger_category.removeAllAppenders();

    current_logger_appender = appender;
    logger_category.setAppender(current_logger_appender);
}

void PSILogger::flush(log4cpp::Priority::Value priorityLevel) {
    logger_category.log(priorityLevel, buffer.str());
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

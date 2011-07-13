#ifndef EXCEPTIONS_HDR
#define EXCEPTIONS_HDR


#include <string>


class Exception {
public:
    Exception(std::string message) : message_(message) { }
    std::string getMessage() { return message_; }
protected:
    std::string message_;
};


class NoEdgeException : public Exception { 
public:
    NoEdgeException(std::string message) : Exception(message) { }
};


#endif
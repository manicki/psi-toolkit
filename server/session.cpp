#include "session.hpp"

#include <time.h>

Session::Session() {
    id_ = generateId();
}

std::string Session::generateId() {
    char buff[20];
    time_t now = time(NULL);
    strftime(buff, 20, "%Y-%m-%d_%H:%M:%S", localtime(&now));

    std::string id = buff;
    id += "_psis";
    return id;
}

std::string Session::getId() {
    return id_;
}

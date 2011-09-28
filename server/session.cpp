#include "session.hpp"

#include <time.h>

Session::Session(std::string id) : id_(id) {}

std::string Session::getId() {
    return id_;
}

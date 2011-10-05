#include "session.hpp"
#include "logging.hpp"

#include <time.h>

Session::Session(std::string sid)
    : id(sid), createdTime(time(NULL))
{}

std::string Session::getId() const {
    return id;
}

void Session::setData(std::string key, std::string value) {
    data[key] = value;
}

std::string Session::getData(std::string key) {
    std::map<std::string, std::string>::iterator it = data.find(key);
    std::string value = "";

    if (it != data.end()) {
        value = it->second;
    }

    return value;
}

bool Session::isData(std::string key) {
    std::map<std::string, std::string>::iterator it = data.find(key);

    if (it != data.end()) {
        if (it->second == "") {
            return false;
        }
        return true;
    }

    return false;
}

void Session::clearData(std::string key) {
    //FIXME
    data[key] = std::string("");
}

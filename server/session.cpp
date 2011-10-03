#include "session.hpp"
#include "logging.hpp"

#include <time.h>

Session::Session(std::string sid)
    : id(sid), createdTime(time(NULL))
{}

std::string Session::getId() {
    return id;
}

void Session::setData(std::string key, std::string value) {
    data[key] = value;
    //INFO("++ currSess: " << key << " = " << data[key] << " dla SID: " << id);
}

std::string Session::getData(std::string key) {
    std::map<std::string, std::string>::iterator it = data.find(key);
    std::string value = "";

    if (it != data.end()) {
        //INFO("+++ znalazlem w getData: " << it->second << " z " << key << " dla SID: " << id);
        value = it->second;
    }

    return value;
}

bool Session::isData(std::string key) {
    std::map<std::string, std::string>::iterator it = data.find(key);

    if (it != data.end()) {
        return true;
    }

    return false;
}

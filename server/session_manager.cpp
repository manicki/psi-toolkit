#include "session_manager.hpp"
#include "logging.hpp"

#include <iostream>

std::string SessionManager::cookieIdentifier = "PSISESSIONID";

std::string SessionManager::getSessionId(std::string cookies) {
    size_t hasId = cookies.find(cookieIdentifier);
    std::string id = "";

    if (hasId != std::string::npos) {
        id = "psis-temp-id";
    }
    return id;
}

std::string SessionManager::CookieHeaderValue(std::string & id) {
    return cookieIdentifier + std::string("=") + id;
}

SessionManager* SessionManager::sessionManagerInstance_ = NULL;

SessionManager* SessionManager::Instance() {
    if (!sessionManagerInstance_) {
        sessionManagerInstance_ = new SessionManager();
    }
    return sessionManagerInstance_;
}

SessionManager::SessionManager() {
    //constructor
}

bool SessionManager::isSession(std::string & id) {
    std::map<std::string, Session>::iterator found = sessions_.find(id);

    if (found != sessions_.end()) {
        return true;
    }
    return false;
}

Session * SessionManager::getSession(std::string & id) {
    return &(sessions_[id]);
}

Session * SessionManager::newSession() {
    Session * ses = new Session();
    addSession(ses);
    return ses;
}

void SessionManager::addSession(Session * session) {
    sessions_.insert(std::pair<std::string, Session> (session->getId(), *session));

    std::string info = "Session IDs: ";
    std::map<std::string, Session>::iterator it;
    for (it = sessions_.begin(); it != sessions_.end(); it++) {
        info += it->first;
        info += ", ";
    }
    INFO(info);
}

#include "session_manager.hpp"
#include "logging.hpp"

#include <iostream>

std::string SessionManager::cookieIdentifier = "PSISESID";
int SessionManager::sessionIdLength = 25;

std::string SessionManager::SessionId(std::string cookies) {
    size_t hasId = cookies.find(cookieIdentifier);

    std::string id = "";
    if (hasId != std::string::npos) {
        id = cookies.substr(hasId + cookieIdentifier.size() + 1, sessionIdLength);
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

SessionManager::SessionManager() : guidGenerator(sessionIdLength, true) {
    currentSessionId = "";
}


bool SessionManager::isSession(std::string & id) {
    std::map<std::string, Session>::iterator found = sessions_.find(id);

    if (found != sessions_.end()) {
        return true;
    }
    return false;
}

Session * SessionManager::getSession(std::string & id) {
    std::map<std::string, Session>::iterator found = sessions_.find(id);

    Session * session = new Session(generateNewId());
    if (found != sessions_.end()) {
        session = &(found->second);
    }
    else {
        ERROR("Attempted to get session that not exists!");
    }
    return session;
}

Session * SessionManager::newSession() {
    std::string id = generateNewId();
    Session * ses = new Session(id);
    addSession(ses);
    return ses;
}

Session * SessionManager::currentSession() {
    return getSession(currentSessionId);
}

void SessionManager::setCurrentSession(std::string id) {
    currentSessionId = id;
}

void SessionManager::addSession(Session * session) {
    sessions_.insert(std::pair<std::string, Session> (session->getId(), *session));

    std::string info = "Session IDs: ";
    std::map<std::string, Session>::iterator it;
    for (it = sessions_.begin(); it != sessions_.end(); ++it) {
        info += it->first;
        info += ", ";
    }
    INFO(info);
}

std::string SessionManager::generateNewId() {
    return guidGenerator.getGUID();
}

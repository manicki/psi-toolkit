#include "session_manager.hpp"
#include "logging.hpp"

#include <iostream>

std::string SessionManager::cookieIdentifier = "PSISESSIONID";
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

SessionManager::SessionManager() {
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
    std::string id = "";

    char time_buff[15];
    time_t now = time(NULL);
    strftime(time_buff, 15, "%y%m%d-%H%M%S-", localtime(&now));
    id += time_buff;

    char rand_buff[sessionIdLength - 15];
    randomString(rand_buff, sessionIdLength - 15);
    id += rand_buff;

    return id;
}

void SessionManager::randomString(char *s, const int len) {
     for (int i = 0; i < len; ++i) {
         int randomChar = rand()%(26+26+10);
         if (randomChar < 26)
             s[i] = 'a' + randomChar;
         else if (randomChar < 26+26)
             s[i] = 'A' + randomChar - 26;
         else
             s[i] = '0' + randomChar - 26 - 26;
     }
     s[len] = 0;
 }


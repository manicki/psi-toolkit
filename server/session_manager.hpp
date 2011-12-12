#ifndef SESSION_MANAGER_HDR
#define SESSION_MANAGER_HDR

#include <string>
#include <map>

#include "session.hpp"
#include "guid_generator.hpp"
#include "server/reply.hpp"

class SessionManager
{

public:

    static SessionManager* Instance();

    static std::string SessionId(std::string cookies);
    static std::string CookieHeaderValue(std::string & id);

    bool isSession(std::string & id);

    Session * getSession(std::string & id);
    Session * newSession();

    Session * currentSession();
    void setCurrentSession(std::string id);

private:

    std::string currentSessionId;

    static std::string cookieIdentifier;
    static int sessionIdLength;
    std::string generateNewId();
    GUIDGenerator guidGenerator;

    std::map<std::string, Session> sessions_;

    void addSession(Session *);

    static SessionManager* sessionManagerInstance_;
    SessionManager();
    SessionManager(const SessionManager&);
    SessionManager& operator= (const SessionManager&);

};

 #endif

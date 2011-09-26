#ifndef SESSION_MANAGER_HDR
#define SESSION_MANAGER_HDR

#include <string>
#include <map>

#include "session.hpp"
#include "server/reply.hpp"

class SessionManager
{

public:

    static SessionManager* Instance();

    bool isSession(std::string & id);

    std::string getSessionId(std::string cookie);
    static std::string CookieHeaderValue(std::string & id);

    Session * getSession(std::string & id);
    Session * newSession();

private:

    static std::string cookieIdentifier;

    std::map<std::string, Session> sessions_;

    static SessionManager* sessionManagerInstance_;
    SessionManager();
    SessionManager(const SessionManager&);
    SessionManager& operator= (const SessionManager&);

    void addSession(Session *);

};

#endif

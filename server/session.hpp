#ifndef SESSION_HDR
#define SESSION_HDR

#include <string>

class Session
{

public:

    Session();
    std::string getId();

private:

    std::string id_;
    std::string generateId();

};

#endif

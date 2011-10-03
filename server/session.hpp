#ifndef SESSION_HDR
#define SESSION_HDR

#include <string>
#include <map>
#include <time.h>

class Session
{

public:

    Session(std::string sid);
    std::string getId();

    void setData(std::string key, std::string value);
    std::string getData(std::string key);
    bool isData(std::string key);

private:

    std::string id;
    time_t createdTime;

    std::map<std::string, std::string> data;

};

#endif

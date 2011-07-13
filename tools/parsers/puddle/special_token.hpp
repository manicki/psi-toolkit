#ifndef SPECIALTOKEN_H__
#define SPECIALTOKEN_H__

#include <string>
#include "entity.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class SpecialToken : public Entity
{
    public:
        SpecialToken();
        SpecialToken(std::string aTokenType);
        ~SpecialToken();

        std::string getType() { return type;}
        void setId(std::string aId);
        std::string getId();
        void setTokenType(std::string aTokenType);
        std::string getTokenType();

        void setCompiled(std::string aCompiledToken);
        std::string getCompiled();
        void updateCompiled();

        std::string toXml();

    private:
        std::string type;

        std::string compiledToken;

        std::string id;
        std::string tokenType; //e.g. sb, se, etc.

};

}

}

}

#endif

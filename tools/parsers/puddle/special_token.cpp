
#include "special_token.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

SpecialToken::SpecialToken()
{
    type = "special";
    id = "";
    tokenType = "";
    compiledToken = "";
}

SpecialToken::SpecialToken(std::string aTokenType)
{
    type = "special";
    tokenType = aTokenType;
    id = "";
    compiledToken = "";
}

SpecialToken::~SpecialToken()
{
//    delete type;
//    delete tokenType;
//    delete id;
//    delete compiledToken;
//    std::cerr << "destrukcja specjala" << std::endl;
}

void SpecialToken::setId(std::string aId)
{
    id = aId;
//    delete id;
//    id = new std::string(aId);
}

std::string SpecialToken::getId()
{
    return id;
}

void SpecialToken::setTokenType(std::string aTokenType)
{
    tokenType = aTokenType;
//    delete tokenType;
//    tokenType = new std::string(aTokenType);
}

std::string SpecialToken::getTokenType()
{
    return tokenType;
}

void SpecialToken::setCompiled(std::string aCompiledToken)
{
    compiledToken = aCompiledToken;
//    delete compiledToken;
//    compiledToken = new std::string(aCompiledToken);
}

std::string SpecialToken::getCompiled()
{
    return compiledToken;
}

void SpecialToken::updateCompiled()
{
    if (tokenType == "sb")
    {
        compiledToken = "<<s<sb<>";
    }
    else if (tokenType == "se")
    {
        compiledToken = "<<s<se<>";
    }
    else
    {
        compiledToken = "";
    }
}

std::string SpecialToken::toXml()
{
    if (tokenType == "sb")
    {
        return ""; //"<sb/>\n";
    }
    else if (tokenType == "se")
    {
        return ""; //<se/>\n";
    }
    else if (tokenType == "ns")
    {
        return "<ns/>\n";
    }
    else
    {
        //jakis inny
        return "";
    }
}

}

}

}

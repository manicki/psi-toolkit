#ifndef TOKEN_H__
#define TOKEN_H__

#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>

#include "utf8_converter.hpp"

#include "entity.hpp"

#include "TransitionInfo.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Token : public Entity // - zrobic jakas abstrakcyjna klase z ktorej bedzie dziedziczyl token, grupa etc?
{
    public:
        Token();
        ~Token();
        std::string getType() { return type;}
        void setCompiled(std::string aCompiledToken);
        //void addMorphology(std::string aMorphology);
        bool addInterpretation(std::string aMorphology, std::string aCompiledInterpretation);
        bool deleteInterpretation(boost::u32regex condition, TransitionInfo* &tok);

        std::string getCompiled();
        std::vector<std::string> getMorphology();
        std::vector<std::string> getCompiledInterpretations();
        std::string getOrth();
        void setOrth(std::string aOrth);

        void clearMorphology();
        void clearCompiledInterpretations();
        void updateCompiled();
        
        void setId(std::string aId);
        void setId(int aId);
        std::string getId();

        std::string toXml();

        std::string getReadable();

    private:
        std::string id;
        std::string orth;
        std::string compiledToken;
        std::vector<std::string> morphology;
        std::vector<std::string> compiledInterpretations;

        std::string type;
        //std::vector<Token*> children;
};

}

}

}

//typedef std::vector<Token> Tokens;

#endif

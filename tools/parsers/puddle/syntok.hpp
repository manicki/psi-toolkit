#ifndef SYNTOK_H__
#define SYNTOK_H__

#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>

#include "utf8_converter.hpp"

#include "entity.hpp"
#include "token.hpp"
#include "group.hpp"

#include "TransitionInfo.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Syntok : public Entity
{
    public:
        Syntok();
        Syntok(Entities &entities, int startIndex, int endIndex, std::string aRuleName);
        ~Syntok();
        std::string getType() { return type; }
        void setCompiled(std::string aCompiled);
        std::string getCompiled();
        bool addInterpretation(std::string aMorphology, std::string aCompiledInterpretation);
        bool deleteInterpretation(boost::u32regex condition, TransitionInfo* &wd);

        std::vector<std::string> getMorphology();
        std::vector<std::string> getCompiledInterpretations();
        std::string getOrth();
        void setOrth(std::string aOrth);
        std::string getId();
        void setId(std::string aId);
        void setId(int aId);

        void clearMorphology();
        void clearCompiledInterpretations();
        void updateCompiled();

        std::string toXml();

        std::string getStart();
        std::string getEnd();
//        static int groupId;
        Entities contents;

        std::string getReadable();

    private:
        std::string type;
        std::string id;
        std::string orth;
        std::vector<std::string> morphology;
        std::vector<std::string> compiledInterpretations;
        std::string compiledSyntok;
        std::string ruleName;
};

}

}

}

#endif


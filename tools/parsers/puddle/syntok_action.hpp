#ifndef SYNTOKACTION_H__
#define SYNTOKACTION_H__

#include "action.hpp"
#include "token.hpp"
#include "syntok.hpp"

#include "add_action.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class SyntokAction : public Action
{
    public:
        SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndexes, std::vector<InterpretationPair> aMorphology, std::string aRuleName, std::string uMorphology);
        ~SyntokAction();
        bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        
        int getStart();
        void setStart(int aStart);
        int getEnd();
        void setEnd(int aEnd);
        std::vector<InterpretationPair> getMorphology();
        void setMorphology(std::vector<InterpretationPair> aMorphology);
        std::vector<int> getTokenIndexes();
        void setTokenIndexes(std::vector<int> aTokenIndexes);
        std::string getRuleName();
        void setRuleName(std::string aRuleName);

        std::string getType() { return type; }

        std::string getUMorphology();

        void setVerbose() { verbose = true; }
        void setSyntok() { syntok = true; }
    private:
        std::vector<int> tokenIndexes;
        std::vector<InterpretationPair> morphology;
        int start, end;

        std::string ruleName;
        std::string type;
        bool verbose;
        bool syntok;

        std::string morphology_;
};

typedef boost::shared_ptr<SyntokAction> SyntokActionPtr;

}

}

}

#endif


#ifndef ADDACTION_H__
#define ADDACTION_H__

#include "action.hpp"
#include <string>
#include <vector>
#include <set>

namespace poleng
{

namespace bonsai
{

namespace puddle
{

//typedef std::pair<std::string, std::string> InterpretationPair;

class AddAction : public Action
{
    public:
        //AddAction(std::vector<InterpretationPair> aInterpretations, std::string aBase, int aTokenIndex, std::string uInterpretation);
        AddAction(std::vector<std::string> aInterpretations, std::string aBase,
                int aTokenIndex, std::string uInterpretation);
        ~AddAction();
        //bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        bool apply(ParseGraphPtr pg, int currentEntity, std::vector<int> matchedTokensSize);
        //bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(ParseGraphPtr pg, int currentEntity, std::vector<int> matchedTokensSize);
        //void setInterpretations(std::vector<InterpretationPair> aInterpretations);
        //std::vector<InterpretationPair> getInterpretations();
        void setInterpretations(std::vector<std::string> aInterpretations);
        std::vector<std::string> getInterpretations();
        void setBase(std::string aBase);
        std::string getBase();
        void setTokenIndex(int aTokenIndex);
        int getTokenIndex();
        std::string getUInterpretation();

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        //std::vector<InterpretationPair> interpretations;
        std::vector<std::string> interpretations;
        std::string base;
        int tokenIndex;
        std::string type;
        bool verbose;
        std::string interpretation_;
};

typedef boost::shared_ptr<AddAction> AddActionPtr;

}

}

}

#endif

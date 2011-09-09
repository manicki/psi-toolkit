#ifndef SYNTOKACTION_H__
#define SYNTOKACTION_H__

#include "action.hpp"
//#include "token.hpp"
//#include "syntok.hpp"

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
        //SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndexes, std::vector<InterpretationPair> aMorphology, std::string aRuleName, std::string uMorphology);
        SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndices,
                std::vector<std::string> aMorphology, std::string aRuleName,
                std::string uMorphology);
        ~SyntokAction();
        //bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        //bool apply(ParseGraphPtr pg, Lattice &lattice, int currentEntity,
        bool apply(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);
        //bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        //bool test(ParseGraphPtr pg, Lattice &lattice, int currentEntity,
        bool test(Lattice&, int, RuleTokenSizes &ruleTokenSizes);

        int getStart() const;
        void setStart(int aStart);
        int getEnd() const;
        void setEnd(int aEnd);
        //std::vector<InterpretationPair> getMorphology();
        //void setMorphology(std::vector<InterpretationPair> aMorphology);
        std::vector<std::string> getMorphology() const;
        void setMorphology(std::vector<std::string> aMorphology);
        std::vector<int> getTokenIndices() const;
        void setTokenIndices(std::vector<int> aTokenIndices);
        std::string getRuleName() const;
        void setRuleName(std::string aRuleName);

        std::string getType() { return type; }

        std::string getUMorphology() const;

        void setVerbose() { verbose = true; }
        void setSyntok() { syntok = true; }
    private:
        std::vector<int> tokenIndices;
        //std::vector<InterpretationPair> morphology;
        std::vector<std::string> morphology;
        size_t start, end;

        std::string ruleName;
        std::string type;
        bool verbose;
        bool syntok;

        std::string morphology_;

//        LatticeWrapperPtr latticeWrapper;
};

typedef boost::shared_ptr<SyntokAction> SyntokActionPtr;

}

}

}

#endif


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
        AddAction(std::vector<Morphology> aInterpretations, std::string aBase,
                int aTokenIndex, std::string uInterpretation);
//                LatticeWrapperPtr aLatticeWrapper);
        ~AddAction();
        //bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        //bool apply(ParseGraphPtr pg, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool apply(Lattice &lattice, int currentEntity, RuleTokenSizes &ruleTokenSizes);
        //bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        //bool test(ParseGraphPtr pg, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(Lattice &lattice, int currentEntity, RuleTokenSizes &ruleTokenSizes);
        //void setInterpretations(std::vector<InterpretationPair> aInterpretations);
        //std::vector<InterpretationPair> getInterpretations();
        void setInterpretations(std::vector<Morphology> aInterpretations);
        std::vector<Morphology> getInterpretations() const;
        void setBase(std::string aBase);
        std::string getBase() const;
        void setTokenIndex(int aTokenIndex);
        int getTokenIndex() const;
        std::string getUInterpretation() const;

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        //std::vector<InterpretationPair> interpretations;
        std::vector<Morphology> interpretations;
        std::string base;
        int tokenIndex;
        std::string type;
        bool verbose;
        std::string interpretation_;

//        LatticeWrapperPtr latticeWrapper;
};

typedef boost::shared_ptr<AddAction> AddActionPtr;

}

}

}

#endif

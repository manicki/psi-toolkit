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

class AddAction : public Action
{
    public:
        AddAction(std::vector<Morphology> aInterpretations, std::string aBase,
                int aTokenIndex);
        ~AddAction();
        bool apply(Lattice &lattice, int currentEntity, RuleTokenSizes &ruleTokenSizes);
        bool test(Lattice &lattice, int currentEntity, RuleTokenSizes &ruleTokenSizes);
        void setInterpretations(std::vector<Morphology> aInterpretations);
        std::vector<Morphology> getInterpretations() const;
        void setBase(std::string aBase);
        std::string getBase() const;
        void setTokenIndex(int aTokenIndex);
        int getTokenIndex() const;

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        void init(std::vector<Morphology> aInterpretations, std::string aBase,
                int aTokenIndex);
        bool checkInterpretationsToAdd(Lattice &lattice,
                Lattice::VertexDescriptor startVertex,
                Lattice::VertexDescriptor endVertex);
        void addEdges(Lattice &lattice, Lattice::VertexDescriptor startVertex,
                Lattice::VertexDescriptor endVertex);
        std::vector<Morphology> interpretations;
        std::string base;
        int tokenIndex;
        std::string type;
        bool verbose;
};

typedef boost::shared_ptr<AddAction> AddActionPtr;

}

}

}

#endif

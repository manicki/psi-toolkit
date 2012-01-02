#ifndef ADDACTION_HDR
#define ADDACTION_HDR

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
        bool apply(Lattice &lattice, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
        bool test(Lattice &lattice, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);

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
};

typedef boost::shared_ptr<AddAction> AddActionPtr;

}

}

}

#endif

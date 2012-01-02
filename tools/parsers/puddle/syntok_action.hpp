#ifndef SYNTOKACTION_HDR
#define SYNTOKACTION_HDR

#include "action.hpp"
#include "add_action.hpp"

namespace poleng {

namespace bonsai {

namespace puddle {

class SyntokAction : public Action {
    public:
        SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndices,
                std::vector<Morphology> aMorphology, std::string aRuleName);
        bool apply(Lattice &lattice, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
        bool test(Lattice &lattice, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);

        void setSyntok() { syntok = true; }
    private:
        void init(int aStart, int aEnd, std::vector<int> aTokenIndices,
                std::vector<Morphology> aMorphology, std::string aRuleName);
        std::vector<std::string> generateBaseForms(Lattice &lattice,
                std::list<Lattice::EdgeSequence> edgeSequences);
        std::string generateOrth(Lattice &lattice,
                std::list<Lattice::EdgeSequence> edgeSequences);

        std::vector<int> tokenIndices;
        std::vector<Morphology> morphology;
        size_t start, end;

        std::string ruleName;
        bool syntok;
};

typedef boost::shared_ptr<SyntokAction> SyntokActionPtr;

}

}

}

#endif

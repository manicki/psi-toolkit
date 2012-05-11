#ifndef UNIFYACTION_HDR
#define UNIFYACTION_HDR

#include "action.hpp"

namespace poleng {

namespace bonsai {

namespace puddle {

class UnifyAction : public Action
{
    public:
        UnifyAction(std::vector<std::string> aUnifiedPatterns,
                std::vector<std::string> aUnifiedAttributes,
                std::vector<int> aTokenIndices);
        bool apply(Lattice &lattice, std::string langCode, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
        bool test(Lattice &lattice, std::string langCode, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);

    private:
        void init(std::vector<std::string> aUnifiedPatterns,
                std::vector<std::string> aUnifiedAttributes,
                std::vector<int> aTokenIndices);
        std::vector<std::vector<std::string> > generateInterpretationsVector(
                Lattice &lattice, std::string langCode,
                RuleTokenSizes &ruleTokenSizes, int matchedStartIndex);
        std::set<std::string> generateUnifiedInterpretationStrings(
                std::vector<std::vector<std::string> > unifiedInterpretations);
        std::vector<std::list<Lattice::EdgeDescriptor> >
            generateUnifiedEdgesList(Lattice &lattice, std::string langCode,
                    RuleTokenSizes &ruleTokenSizes, int CurrentEntity,
                    std::set<std::string> unifiedStrings);
        bool isUnifyingPossible(Lattice &lattice, std::string langCode,
                int matchedStartIndex, RuleTokenSizes &ruleTokenSizes);
        void discardUnunifiedEdges(Lattice &lattice, std::string langCode,
                RuleTokenSizes &ruleTokenSizes, int matchedStartIndex,
                std::vector<std::list<Lattice::EdgeDescriptor> > edgesToKeep);

        std::vector<int> tokenIndices;

        bool nullAgreement; // true = jak w czesci mowy atrybut nie wystepuje,
        //to traktuj jako dajacy sie z unifikowac,
        //false = jak w czesci mowy nie ma atrybutu, to nie da sie zunifikowac
        std::vector<std::string> unifiedPatterns;
        std::vector<std::string> unifiedAttributes;
};

typedef boost::shared_ptr<UnifyAction> UnifyActionPtr;

}

}

}

#endif

#ifndef UNIFYACTION_H__
#define UNIFYACTION_H__

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

class UnifyAction : public Action
{
    public:
        UnifyAction(std::vector<std::string> aUnifiedPatterns,
                std::vector<std::string> aUnifiedAttributes,
                std::vector<int> aTokenIndices);
        bool apply(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);
        bool test(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);

    private:
        void init(std::vector<std::string> aUnifiedPatterns,
                std::vector<std::string> aUnifiedAttributes,
                std::vector<int> aTokenIndices);
        std::vector<int> tokenIndices;

        bool nullAgreement; // true = jak w czesci mowy atrybut nie wystepuje, to traktuj jako dajacy sie z unifikowac, false = jak w czesci mowy nie ma atrybutu, to nie da sie zunifikowac
        std::vector<std::string> unifiedPatterns;
        std::vector<std::string> unifiedAttributes;
};

typedef boost::shared_ptr<UnifyAction> UnifyActionPtr;

}

}

}

#endif

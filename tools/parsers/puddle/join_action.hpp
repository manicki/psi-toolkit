#ifndef JOINACTION_H__
#define JOINACTION_H__

#include "action.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class JoinAction : public Action
{
    public:
        JoinAction(std::string aGroup, int aStart, int aEnd, int aHead, std::string aRuleName);
        bool apply(Lattice &lattice, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
        bool test(Lattice &lattice, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);

    private:
        void init(std::string aGroup, int aStart, int aEnd, int aHead,
                std::string aRuleName);
        std::string group;
        size_t head;
        size_t start, end;
        std::string ruleName;
};

typedef boost::shared_ptr<JoinAction> JoinActionPtr;

}

}

}

#endif

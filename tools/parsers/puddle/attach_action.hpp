#ifndef ATTACHACTION_H__
#define ATTACHACTION_H__

#include "action.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class AttachAction : public Action
{
    public:
        AttachAction(std::string aGroup, int aStart, int aEnd, int aHead,
                std::string aRuleName);
        bool apply(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
        bool test(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
//        bool apply(Lattice &lattice, int currentEntity,
//                RuleTokenSizes &ruleTokenSizes);
//        bool test(Lattice &lattice, int, RuleTokenSizes &ruleTokenSizes);

    private:
        void init(std::string aGroup, int aStart, int aEnd, int aHead,
                std::string aRuleName);
        std::string group;
        size_t head;
        size_t start, end;
        std::string ruleName;
};

typedef boost::shared_ptr<AttachAction> AttachActionPtr;

}

}

}

#endif

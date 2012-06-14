#ifndef ATTACHACTION_HDR
#define ATTACHACTION_HDR

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
        bool apply(Lattice &lattice, std::string langCode, int matchedStartIndex,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
        bool test(Lattice &lattice, std::string langCode, int matchedStartIndex,
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

typedef boost::shared_ptr<AttachAction> AttachActionPtr;

}

}

}

#endif

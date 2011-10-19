#ifndef TRANSFORMACTION_H__
#define TRANSFORMACTION_H__

#include "action.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class TransformAction : public Action
{
    public:
        TransformAction(std::string aGroup, int aElement, std::string aRuleName);
        ~TransformAction();
        bool apply(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);
        bool test(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);

    private:
        std::string group;
        int element;
        std::string ruleName;
};

typedef boost::shared_ptr<TransformAction> TransformActionPtr;

}

}

}

#endif

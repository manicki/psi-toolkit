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
        std::string getGroup() const;
        int getElement() const;
        void setGroup(std::string aGroup);
        void setElement(int aElement);
        std::string getRuleName() const;
        void setRuleName(std::string aRuleName);

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        std::string group;
        int element;
        std::string ruleName;
        std::string type;
        bool verbose;
};

typedef boost::shared_ptr<TransformAction> TransformActionPtr;

}

}

}

//typedef std::vector<TransformAction> Actions;

#endif

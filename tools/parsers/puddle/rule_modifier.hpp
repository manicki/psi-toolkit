#ifndef RULEMODIFIER_H__
#define RULEMODIFIER_H__

#include "rule_loader.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

class RuleModifier
{
    public:
        RuleModifier();
        ~RuleModifier();
        void addRule(std::string ruleString);//, LatticeWrapperPtr latticeWrapper);
        void addRule(RulePtr rule);
        void deleteRule(size_t index);
        void moveRule(size_t from, size_t to);

        void setRules(RulesPtr aRules);
        void setRuleCompiler(RuleLoader *aRuleCompiler);
        RulesPtr getRules() const;

        void addAction(size_t ruleIndex, ActionPtr action);
        void addAction(size_t ruleIndex, std::string actionString);
//                LatticeWrapperPtr latticeWrapper);
        void deleteAction(size_t ruleIndex, size_t actionIndex);

        void setRepeat(size_t ruleIndex);
        void unsetRepeat(size_t ruleIndex);

        void setMatch(size_t ruleIndex, std::string match);
        void setLeftContext(size_t ruleIndex, std::string context);
        void setRightContext(size_t ruleIndex, std::string context);

    private:

        RulesPtr rules;
        RuleLoader *ruleCompiler;
};

}

}

}

#endif


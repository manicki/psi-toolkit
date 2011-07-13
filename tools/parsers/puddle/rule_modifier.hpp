#ifndef RULEMODIFIER_H__
#define RULEMODIFIER_H__

#include "rule_compiler.hpp"

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
        void addRule(std::string ruleString);
        void addRule(RulePtr rule);
        void deleteRule(int index);
        void moveRule(int from, int to);
        
        void setRules(RulesPtr aRules);
        void setRuleCompiler(RuleCompiler *aRuleCompiler);
        RulesPtr getRules();

        void addAction(int ruleIndex, ActionPtr action);
        void addAction(int ruleIndex, std::string actionString);
        void deleteAction(int ruleIndex, int actionIndex);

        void setRepeat(int ruleIndex);
        void unsetRepeat(int ruleIndex);

        void setMatch(int ruleIndex, std::string match);
        void setLeftContext(int ruleIndex, std::string context);
        void setRightContext(int ruleIndex, std::string context);

    private:

        RulesPtr rules;
        RuleCompiler *ruleCompiler;
};

}

}

}

#endif


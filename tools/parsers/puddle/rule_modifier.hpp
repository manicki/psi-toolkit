#ifndef RULEMODIFIER_HDR
#define RULEMODIFIER_HDR

#include "rule_loader.hpp"

namespace poleng {

namespace bonsai {
    namespace puddle {

class RuleModifier {
    public:
        RuleModifier();
        void addRule(std::string ruleString);
        void addRule(RulePtr rule);
        void deleteRule(size_t index);
        void moveRule(size_t from, size_t to);

        void setRules(RulesPtr aRules);
        void setRuleCompiler(RuleLoader *aRuleCompiler);
        RulesPtr getRules() const;

        void addAction(size_t ruleIndex, ActionPtr action);
        void addAction(size_t ruleIndex, std::string actionString);
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

typedef boost::shared_ptr<RuleModifier> RuleModifierPtr;

}

}

}

#endif

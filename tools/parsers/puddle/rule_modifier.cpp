
#include "rule_modifier.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

RuleModifier::RuleModifier() {
    ruleCompiler = NULL;
}

void RuleModifier::addRule(std::string ruleString) {
    RulePtr rule = ruleCompiler->compileRule(ruleString);;
    rules->push_back(rule);
}

void RuleModifier::addRule(RulePtr rule) {
    rules->push_back(rule);
}

void RuleModifier::deleteRule(size_t index) {
    if (index < rules->size()) {
        rules->erase(rules->begin() + index);
    }
}

void RuleModifier::moveRule(size_t from, size_t to) {
    RulePtr tmp = rules->at(from);
    int from_ = 0;
    int to_ = 0;

    if (to < from)
        from_ ++;
    if (to > from)
        to_ ++;

    rules->insert(rules->begin() + to + to_, tmp);
    rules->erase(rules->begin() + from + from_);
}

void RuleModifier::setRules(RulesPtr aRules) {
    rules = aRules;
}

void RuleModifier::setRuleCompiler(RuleLoader *aRuleCompiler) {
    ruleCompiler = aRuleCompiler;
}

RulesPtr RuleModifier::getRules() const {
    return rules;
}

void RuleModifier::addAction(size_t ruleIndex, ActionPtr action) {
    if (ruleIndex < rules->size()) {
        rules->at(ruleIndex)->addAction(action);
    }
}

void RuleModifier::addAction(size_t ruleIndex, std::string actionString) {
    if (ruleIndex < rules->size()) {
        ActionPtr action = ruleCompiler->compileAction(actionString,
                rules->at(ruleIndex));
        rules->at(ruleIndex)->addAction(action);
    }
}

void RuleModifier::deleteAction(size_t ruleIndex, size_t actionIndex) {
    if (ruleIndex < rules->size()) {
        rules->at(ruleIndex)->deleteAction(actionIndex);
    }
}

void RuleModifier::setRepeat(size_t ruleIndex) {
    if (ruleIndex < rules->size()) {
        rules->at(ruleIndex)->setRepeat(true);
    }
}

void RuleModifier::unsetRepeat(size_t ruleIndex) {
    if (ruleIndex < rules->size()) {
        rules->at(ruleIndex)->setRepeat(false);
    }
}

void RuleModifier::setMatch(size_t ruleIndex, std::string match) {
    if (ruleIndex < rules->size()) {
        RuleTokenPatterns ruleTokenPatterns;
        RuleTokenModifiers ruleTokenModifiers;
        RuleTokenRequirements ruleTokenRequirements;
        RulePatternIndices rulePatternIndices;
#if HAVE_RE2
        NegativePatternStrings negativePatterns;
#endif
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        if (rules->at(ruleIndex)->getLeftCount() > 0) {
            std::string tmp = rules->at(ruleIndex)->getLeft();
#if HAVE_RE2
            compiled = ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount, negativePatterns);
#else
            compiled = ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount);
#endif
        }
#if HAVE_RE2
        compiled += ruleCompiler->compileRulePattern(match, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount, negativePatterns);
#else
        compiled += ruleCompiler->compileRulePattern(match, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount);
#endif
        rules->at(ruleIndex)->setMatchCount(size);
        if (rules->at(ruleIndex)->getRightCount() > 0) {
            std::string tmp = rules->at(ruleIndex)->getRight();
#if HAVE_RE2
            compiled += ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount, negativePatterns);
#else
            compiled += ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount);
#endif
        }
        rules->at(ruleIndex)->setMatch(match);
        rules->at(ruleIndex)->setRuleTokenPatterns(ruleTokenPatterns);
        rules->at(ruleIndex)->setRuleTokenModifiers(ruleTokenModifiers);
        rules->at(ruleIndex)->setRuleTokenRequirements(ruleTokenRequirements);
        rules->at(ruleIndex)->setRulePatternIndices(rulePatternIndices);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

void RuleModifier::setLeftContext(size_t ruleIndex, std::string context) {
    if (ruleIndex < rules->size()) {
        RuleTokenPatterns ruleTokenPatterns;
        RuleTokenModifiers ruleTokenModifiers;
        RuleTokenRequirements ruleTokenRequirements;
        RulePatternIndices rulePatternIndices;
#if HAVE_RE2
        NegativePatternStrings negativePatterns;
#endif
        int bracketCount = 0;
        int size;
        std::string compiled = "";
#if HAVE_RE2
        compiled = ruleCompiler->compileRulePattern(context, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount, negativePatterns);
#else
        compiled = ruleCompiler->compileRulePattern(context, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount);
#endif
        rules->at(ruleIndex)->setLeftCount(size);
        std::string tmp = rules->at(ruleIndex)->getMatch();
#if HAVE_RE2
        compiled += ruleCompiler->compileRulePattern(tmp, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount, negativePatterns);
#else
        compiled += ruleCompiler->compileRulePattern(tmp, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount);
#endif
        if (rules->at(ruleIndex)->getRightCount() > 0) {
            std::string tmp = rules->at(ruleIndex)->getRight();
#if HAVE_RE2
            compiled += ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount, negativePatterns);
#else
            compiled += ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount);
#endif
        }
        rules->at(ruleIndex)->setLeft(context);
        rules->at(ruleIndex)->setRuleTokenPatterns(ruleTokenPatterns);
        rules->at(ruleIndex)->setRuleTokenModifiers(ruleTokenModifiers);
        rules->at(ruleIndex)->setRuleTokenRequirements(ruleTokenRequirements);
        rules->at(ruleIndex)->setRulePatternIndices(rulePatternIndices);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

void RuleModifier::setRightContext(size_t ruleIndex, std::string context) {
    if (ruleIndex < rules->size()) {
        RuleTokenPatterns ruleTokenPatterns;
        RuleTokenModifiers ruleTokenModifiers;
        RuleTokenRequirements ruleTokenRequirements;
        RulePatternIndices rulePatternIndices;
#if HAVE_RE2
        NegativePatternStrings negativePatterns;
#endif
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        if (rules->at(ruleIndex)->getLeftCount() > 0) {
            std::string tmp = rules->at(ruleIndex)->getLeft();
#if HAVE_RE2
            compiled = ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount, negativePatterns);
#else
            compiled = ruleCompiler->compileRulePattern(tmp, size,
                    ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                    rulePatternIndices, bracketCount);
#endif
        }
        std::string tmp = rules->at(ruleIndex)->getMatch();
#if HAVE_RE2
        compiled += ruleCompiler->compileRulePattern(tmp, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount, negativePatterns);
#else
        compiled += ruleCompiler->compileRulePattern(tmp, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount);
#endif
#if HAVE_RE2
        compiled += ruleCompiler->compileRulePattern(context, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount, negativePatterns);
#else
        compiled += ruleCompiler->compileRulePattern(context, size,
                ruleTokenPatterns, ruleTokenModifiers, ruleTokenRequirements,
                rulePatternIndices, bracketCount);
#endif
        rules->at(ruleIndex)->setRightCount(size);
        rules->at(ruleIndex)->setRight(context);
        rules->at(ruleIndex)->setRuleTokenPatterns(ruleTokenPatterns);
        rules->at(ruleIndex)->setRuleTokenModifiers(ruleTokenModifiers);
        rules->at(ruleIndex)->setRuleTokenRequirements(ruleTokenRequirements);
        rules->at(ruleIndex)->setRulePatternIndices(rulePatternIndices);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

}

}

}

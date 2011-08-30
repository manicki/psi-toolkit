
#include "rule_modifier.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

RuleModifier::RuleModifier()
{
    ruleCompiler = NULL;
}

RuleModifier::~RuleModifier()
{
}

void RuleModifier::addRule(std::string ruleString)//,
//        LatticeWrapperPtr latticeWrapper)
{
    RulePtr rule = ruleCompiler->compileRule(ruleString);//, latticeWrapper);
    rules->push_back(rule);
}

void RuleModifier::addRule(RulePtr rule)
{
    rules->push_back(rule);
}

void RuleModifier::deleteRule(size_t index)
{
    if (index < rules->size())
    {
        rules->erase(rules->begin() + index);
    }
}

void RuleModifier::moveRule(size_t from, size_t to)
{
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

void RuleModifier::setRules(RulesPtr aRules)
{
    rules = aRules;
}

void RuleModifier::setRuleCompiler(RuleLoader *aRuleCompiler)
{
    ruleCompiler = aRuleCompiler;
}

RulesPtr RuleModifier::getRules() const {
    return rules;
}

void RuleModifier::addAction(size_t ruleIndex, ActionPtr action)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->addAction(action);
    }
}

void RuleModifier::addAction(size_t ruleIndex, std::string actionString)//,
//        LatticeWrapperPtr latticeWrapper)
{
    if (ruleIndex < rules->size())
    {
        ActionPtr action = ruleCompiler->compileAction(actionString,
                rules->at(ruleIndex));//, latticeWrapper);
        rules->at(ruleIndex)->addAction(action);
    }
}

void RuleModifier::deleteAction(size_t ruleIndex, size_t actionIndex)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->deleteAction(actionIndex);
    }
}

void RuleModifier::setRepeat(size_t ruleIndex)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->setRepeat(true);
    }
}

void RuleModifier::unsetRepeat(size_t ruleIndex)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->setRepeat(false);
    }
}

void RuleModifier::setMatch(size_t ruleIndex, std::string match)
{
    if (ruleIndex < rules->size())
    {
        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedIndices;
        NegativePatternStrings negativePatterns;
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        if (rules->at(ruleIndex)->getLeftCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getLeft();
            compiled = ruleCompiler->compileRulePattern(tmp, size,
                    tokensPatterns, tokensModifiers, tokensRequired,
                    matchedIndices, bracketCount, negativePatterns);
        }
        compiled += ruleCompiler->compileRulePattern(match, size,
                tokensPatterns, tokensModifiers, tokensRequired,
                matchedIndices, bracketCount, negativePatterns);
        rules->at(ruleIndex)->setMatchCount(size);
        if (rules->at(ruleIndex)->getRightCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getRight();
            compiled += ruleCompiler->compileRulePattern(tmp, size,
                    tokensPatterns, tokensModifiers, tokensRequired,
                    matchedIndices, bracketCount, negativePatterns);
        }
        rules->at(ruleIndex)->setMatch(match);
        rules->at(ruleIndex)->setTokensPatterns(tokensPatterns);
        rules->at(ruleIndex)->setTokensModifiers(tokensModifiers);
        rules->at(ruleIndex)->setTokensRequired(tokensRequired);
        rules->at(ruleIndex)->setMatchedIndices(matchedIndices);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

void RuleModifier::setLeftContext(size_t ruleIndex, std::string context)
{
    if (ruleIndex < rules->size())
    {
        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedIndices;
        NegativePatternStrings negativePatterns;
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        compiled = ruleCompiler->compileRulePattern(context, size,
                tokensPatterns, tokensModifiers, tokensRequired,
                matchedIndices, bracketCount, negativePatterns);
        rules->at(ruleIndex)->setLeftCount(size);
        std::string tmp = rules->at(ruleIndex)->getMatch();
        compiled += ruleCompiler->compileRulePattern(tmp, size,
                tokensPatterns, tokensModifiers, tokensRequired,
                matchedIndices, bracketCount, negativePatterns);
        if (rules->at(ruleIndex)->getRightCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getRight();
            compiled += ruleCompiler->compileRulePattern(tmp, size,
                    tokensPatterns, tokensModifiers, tokensRequired,
                    matchedIndices, bracketCount, negativePatterns);
        }
        rules->at(ruleIndex)->setLeft(context);
        rules->at(ruleIndex)->setTokensPatterns(tokensPatterns);
        rules->at(ruleIndex)->setTokensModifiers(tokensModifiers);
        rules->at(ruleIndex)->setTokensRequired(tokensRequired);
        rules->at(ruleIndex)->setMatchedIndices(matchedIndices);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

void RuleModifier::setRightContext(size_t ruleIndex, std::string context)
{
    if (ruleIndex < rules->size())
    {
        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedIndices;
        NegativePatternStrings negativePatterns;
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        if (rules->at(ruleIndex)->getLeftCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getLeft();
            compiled = ruleCompiler->compileRulePattern(tmp, size,
                    tokensPatterns, tokensModifiers, tokensRequired,
                    matchedIndices, bracketCount, negativePatterns);
        }
        std::string tmp = rules->at(ruleIndex)->getMatch();
        compiled += ruleCompiler->compileRulePattern(tmp, size,
                tokensPatterns, tokensModifiers, tokensRequired,
                matchedIndices, bracketCount, negativePatterns);
        compiled += ruleCompiler->compileRulePattern(context, size,
                tokensPatterns, tokensModifiers, tokensRequired,
                matchedIndices, bracketCount, negativePatterns);
        rules->at(ruleIndex)->setRightCount(size);
        rules->at(ruleIndex)->setRight(context);
        rules->at(ruleIndex)->setTokensPatterns(tokensPatterns);
        rules->at(ruleIndex)->setTokensModifiers(tokensModifiers);
        rules->at(ruleIndex)->setTokensRequired(tokensRequired);
        rules->at(ruleIndex)->setMatchedIndices(matchedIndices);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

}

}

}


#include "rule_modifier.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

RuleModifier::RuleModifier()
{
}

RuleModifier::~RuleModifier()
{
}

void RuleModifier::addRule(std::string ruleString)
{
    RulePtr rule = ruleCompiler->compileRule(ruleString);
    rules->push_back(rule);
}

void RuleModifier::addRule(RulePtr rule)
{
    rules->push_back(rule);
}

void RuleModifier::deleteRule(int index)
{
    if (index < rules->size())
    {
        rules->erase(rules->begin() + index);
    }
}

void RuleModifier::moveRule(int from, int to)
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

RulesPtr RuleModifier::getRules()
{
    return rules;
}

void RuleModifier::addAction(int ruleIndex, ActionPtr action)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->addAction(action);
    }
}

void RuleModifier::addAction(int ruleIndex, std::string actionString)
{
    if (ruleIndex < rules->size())
    {
        ActionPtr action = ruleCompiler->compileAction(actionString, rules->at(ruleIndex));
        rules->at(ruleIndex)->addAction(action);
    }
}

void RuleModifier::deleteAction(int ruleIndex, int actionIndex)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->deleteAction(actionIndex);
    }
}

void RuleModifier::setRepeat(int ruleIndex)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->setRepeat(true);
    }
}

void RuleModifier::unsetRepeat(int ruleIndex)
{
    if (ruleIndex < rules->size())
    {
        rules->at(ruleIndex)->setRepeat(false);
    }
}

void RuleModifier::setMatch(int ruleIndex, std::string match)
{
    if (ruleIndex < rules->size())
    {
        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedIndexes;
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        if (rules->at(ruleIndex)->getLeftCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getLeft();
            compiled = ruleCompiler->compileRulePattern(tmp, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        }
        compiled += ruleCompiler->compileRulePattern(match, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        rules->at(ruleIndex)->setMatchCount(size);
        if (rules->at(ruleIndex)->getRightCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getRight();
            compiled += ruleCompiler->compileRulePattern(tmp, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        }
        rules->at(ruleIndex)->setMatch(match);
        rules->at(ruleIndex)->setTokensPatterns(tokensPatterns);
        rules->at(ruleIndex)->setTokensModifiers(tokensModifiers);
        rules->at(ruleIndex)->setTokensRequired(tokensRequired);
        rules->at(ruleIndex)->setMatchedIndexes(matchedIndexes);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

void RuleModifier::setLeftContext(int ruleIndex, std::string context)
{
    if (ruleIndex < rules->size())
    {
        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedIndexes;
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        compiled = ruleCompiler->compileRulePattern(context, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        rules->at(ruleIndex)->setLeftCount(size);
        std::string tmp = rules->at(ruleIndex)->getMatch();
        compiled += ruleCompiler->compileRulePattern(tmp, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        if (rules->at(ruleIndex)->getRightCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getRight();
            compiled += ruleCompiler->compileRulePattern(tmp, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        }
        rules->at(ruleIndex)->setLeft(context);
        rules->at(ruleIndex)->setTokensPatterns(tokensPatterns);
        rules->at(ruleIndex)->setTokensModifiers(tokensModifiers);
        rules->at(ruleIndex)->setTokensRequired(tokensRequired);
        rules->at(ruleIndex)->setMatchedIndexes(matchedIndexes);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

void RuleModifier::setRightContext(int ruleIndex, std::string context)
{
    if (ruleIndex < rules->size())
    {
        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedIndexes;
        int bracketCount = 0;
        int size;
        std::string compiled = "";
        if (rules->at(ruleIndex)->getLeftCount() > 0)
        {
            std::string tmp = rules->at(ruleIndex)->getLeft();
            compiled = ruleCompiler->compileRulePattern(tmp, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        }
        std::string tmp = rules->at(ruleIndex)->getMatch();
        compiled += ruleCompiler->compileRulePattern(tmp, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        compiled += ruleCompiler->compileRulePattern(context, size, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
        rules->at(ruleIndex)->setRightCount(size);
        rules->at(ruleIndex)->setRight(context);
        rules->at(ruleIndex)->setTokensPatterns(tokensPatterns);
        rules->at(ruleIndex)->setTokensModifiers(tokensModifiers);
        rules->at(ruleIndex)->setTokensRequired(tokensRequired);
        rules->at(ruleIndex)->setMatchedIndexes(matchedIndexes);
        rules->at(ruleIndex)->setPattern(compiled);
    }
}

}

}

}

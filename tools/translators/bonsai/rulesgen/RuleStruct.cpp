
#include "RuleStruct.hpp"

#include <iostream>

namespace poleng
{

namespace bonsai
{

RuleStruct::RuleStruct()
{
    src_sentence = "";
    trg_sentence = "";
    alignment = "";
}

RuleStruct::RuleStruct(std::string aSrcSentence, std::string aTrgSentence, std::string aAlignment)
{
    src_sentence = aSrcSentence;
    trg_sentence = aTrgSentence;
    alignment = aAlignment;
}

RuleStruct::~RuleStruct()
{
}

std::string RuleStruct::getSrcSentence()
{
    return src_sentence;
}

std::string RuleStruct::getTrgSentence()
{
    return trg_sentence;
}

std::string RuleStruct::getAlignment()
{
    return alignment;
}

std::vector<std::string> RuleStruct::getRules()
{
    return rules;
}

void RuleStruct::addRule(std::string &rule)
{
    rules.push_back(rule);
}

void RuleStruct::setSrcSentence(std::string &aSentence)
{
    src_sentence = aSentence;
}

void RuleStruct::setTrgSentence(std::string &aSentence)
{
    trg_sentence = aSentence;
}

void RuleStruct::setAlignment(std::string aAlignment)
{
    alignment = aAlignment;
}

}

}


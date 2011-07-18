#ifndef RULECOMPILER_H__
#define RULECOMPILER_H__

#include <string>
#include <vector>
#include "rule.hpp"
#include "action.hpp"
#include "tagset.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

typedef std::vector<std::string> TokenMask;
typedef std::vector<TokenMask> Interpretations;

class RuleCompiler
{
    public:
        RuleCompiler();
        RuleCompiler(std::string &filename);
        ~RuleCompiler();

        void setTagset(TagsetPtr aTagset);
        RulesPtr readFromFile(std::string &filename);
//        void test();

//        void logRulesToFile(std::string &filename);

        void setVerbose();
        void setSyntok();

        RulePtr compileRule(std::string ruleString);
        ActionPtr compileAction(std::string actionString, RulePtr rule);

        std::string compileRuleMatch(std::string &matched, int &size, std::vector<std::string> &tokensPatterns, std::vector<std::string> &tokensModifiers, std::vector<bool> &tokensRequired, std::vector<int> &matchedIndexes, int &bracketCount);

    private:
        TagsetPtr tagset;
        int numAttributes;
        std::string compileNonTokens(std::string &matched);
        std::string compileRuleName(std::string matched);
        //std::string compileRuleMatch(std::string &matched, int &size, std::vector<string> &tokensPatterns, std::vector<string> &tokensModifiers, std::vector<bool> &tokensRequired, std::vector<int> &matchedIndexes, int &bracketCount);
        ActionsPtr compileRuleAction(std::string &matched, int ruleLeftCount, int ruleMatchCount, int ruleRightCount, std::string ruleName, bool &repeat);

        int countTokens(std::string &matched);
        std::string getToken(std::string &matched, std::string &before);
        std::string compileToken(std::string &token);
        std::string getKey(std::string &token);
        std::string getValue(std::string &token);
        std::string getOperator(std::string &token);

        bool compilePosCondition(std::string &comparisonOperator, std::string &value, Interpretations &interps);
        bool compileBaseCondition(std::string &comparisonOperator, std::string &value, Interpretations &interps, bool icase);
        bool compileAttributeCondition(std::string &key, std::string &comparisonOperator, std::string &value, Interpretations &interps);
        std::string compileOrthCondition(std::string &comparisonOperator, std::string &value, bool icase);
        std::string interpretationsToString(Interpretations interps);
        bool compileDeleteCondition(std::string &key, std::string &comparisonOperator, std::string &value, TokenMask &mask, std::string &orth);
        bool compileAddInterpretation(std::string &pattern, std::vector<InterpretationPair> &interpretations);

        TokenMask baseMask;
        int baseIndex;
        int modifierIndex;

        std::string nothingSet;
        boost::u32regex regNothingSet;

        bool verbose;
        bool syntok;

        RulesPtr rules;
//        RulesPtr rules;
        //std::string trim(std::string s);
       // std::string baseMask;

        boost::regex regAmp;
        boost::regex regLt;
        boost::regex regGt;
        boost::regex regLPar;
        boost::regex regRPar;
        boost::regex regAlt, regPlus, regAsterisk, regOpt;
};

}

}

}

//namespace mynamespace
//{
//    class utf8_codecvt_facet
//    {
//    };
//
//}

#endif

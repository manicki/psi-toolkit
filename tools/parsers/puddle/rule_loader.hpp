#ifndef RULELOADER_H__
#define RULELOADER_H__

#include <string>
#include <vector>
#include "rule.hpp"
#include "action.hpp"
#include "tagset.hpp"

#include "puddle_types.hpp"

namespace poleng {

namespace bonsai {

namespace puddle {

class TokenPatternPart {
    public:
        TokenPatternPart() : condition(""), negative(false) {}
        std::string condition;
        bool negative;
};
class TokenPattern {
    public:
        TokenPattern() : modifier("") {}
        std::vector<TokenPatternPart> parts;
        std::string modifier;
        TokenPatternPart base;
};
typedef std::vector<TokenPattern> TokenPatterns;

class RuleLoader {
    public:
        RuleLoader();

        void setTagset(TagsetPtr aTagset);
        RulesPtr readFromFile(std::string &filename);

        void setVerbose(); //@todo: te do wyrzutu chyba
        void setSyntok();

        RulePtr compileRule(std::string ruleString);
        ActionPtr compileAction(std::string actionString, RulePtr rule);

#if HAVE_RE2
        std::string compileRulePattern(std::string &matched, int &size,
                RuleTokenPatterns &ruleTokenPatterns,
                RuleTokenModifiers &ruleTokenModifiers,
                RuleTokenRequirements &ruleTokenRequirements,
                RulePatternIndices &rulePatternIndices, int &bracketCount,
                NegativePatternStrings &negativePatterns);
#else
        std::string compileRulePattern(std::string &matched, int &size,
                RuleTokenPatterns &ruleTokenPatterns,
                RuleTokenModifiers &ruleTokenModifiers,
                RuleTokenRequirements &ruleTokenRequirements,
                RulePatternIndices &rulePatternIndices, int &bracketCount);
#endif

    private:
        std::string compileNonTokens(std::string &matched);
        std::string compileRuleName(std::string matched);
        ActionsPtr compileRuleAction(std::string &matched, int ruleLeftCount,
                int ruleMatchCount, int ruleRightCount, std::string ruleName,
                bool &repeat);

        int countTokens(std::string &matched);
        std::string getToken(std::string &matched, std::string &before);
#if HAVE_RE2
        std::string compileToken(std::string &token,
                NegativePatternStrings &negativePatterns, bool no_prefix = false);
#else
        std::string compileToken(std::string &token, bool no_prefix = false);
#endif

        std::string getKey(std::string &token);
        std::string getValue(std::string &token);
        std::string getOperator(std::string &token);
        std::string getHead(std::string &token);

        void compilePosCondition(std::string &comparisonOperator,
                std::string &value, TokenPatterns &tokenPatterns);
        void compileBaseCondition(std::string &comparisonOperator,
                std::string &value, TokenPatterns &tokenPatterns, bool icase);
        void compileAttributeCondition(std::string &key,
                std::string &comparisonOperator, std::string &value,
                TokenPatterns &tokenPatterns);
#if HAVE_RE2
        std::string compileOrthCondition(std::string &comparisonOperator,
                std::string &value, bool icase,
                NegativePatternStrings &negativePatterns);
#else
        std::string compileOrthCondition(std::string &comparisonOperator,
                std::string &value, bool icase);
#endif
        bool compileDeleteCondition(std::string &key, std::string &comparisonOperator,
                std::string &value, DeleteConditions &conditions);
        std::vector<Morphology> compileAddInterpretation(std::string &pattern);

#if HAVE_RE2
        std::string generateTokenPatternsString(TokenPatterns tokenPatterns,
                NegativePatternStrings &negativePatterns);
#else
        std::string generateTokenPatternsString(TokenPatterns tokenPatterns);
#endif
#if HAVE_RE2
        std::string generateCompiledTokenString(bool tokenMatch,
                std::string &type, std::string &compiledHead, std::string &orth,
                TokenPatterns tokenPatterns, NegativePatternStrings &negativePatterns,
                bool no_prefix);
#else
        std::string generateCompiledTokenString(bool tokenMatch,
                std::string &type, std::string &compiledHead, std::string &orth,
                TokenPatterns tokenPatterns, bool no_prefix);
#endif

        DeleteActionPtr createDeleteAction(std::string &conditionsString,
                int tokenIndex);
        AddActionPtr createAddAction(std::string &interpretation,
                std::string &baseString, int tokenIndex);
        UnifyActionPtr createUnifyAction(std::string &attributesString,
                std::string &tokensString);
        SyntokActionPtr createSyntokAction(std::string &interpretationString,
                int ruleLeftSize, int ruleMatchSize, std::string &ruleName);

        void addConditionToPatterns(TokenPatterns &tokenPatterns,
                std::string conditionString, int attributeIndex,
                std::string comparisonOperator);

        RulePtr parseRuleString(std::string &ruleString, bool &wrong);

        TagsetPtr tagset;
        bool verbose;
        bool syntok;
        RulesPtr rules;

        std::string escapeSpecialChars(std::string text);

        void initProperties();
};

}

}

}

#endif

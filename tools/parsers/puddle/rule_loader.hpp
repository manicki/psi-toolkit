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

//typedef std::vector<std::string> TokenMask;
//typedef std::vector<TokenMask> Interpretations;

class TokenPatternPart {
    public:
        TokenPatternPart() : condition(""), negative(false) {}
        std::string condition;
//        std::string modifier;
        bool negative;
};
class TokenPattern {
    public:
        TokenPattern() : modifier("") {}
        std::vector<TokenPatternPart> parts;
        std::string modifier;
        TokenPatternPart base;
};
//typedef std::vector<TokenPatternPart> TokenPattern;
typedef std::vector<TokenPattern> TokenPatterns;

class RuleLoader {
    public:
        RuleLoader();
//        RuleLoader(std::string &filename);
        ~RuleLoader();

        void setTagset(TagsetPtr aTagset);
        RulesPtr readFromFile(std::string &filename, LatticeWrapperPtr latticeWrapper);
//        void test();

//        void logRulesToFile(std::string &filename);

        void setVerbose();
        void setSyntok();

        RulePtr compileRule(std::string ruleString, LatticeWrapperPtr latticeWrapper);
        ActionPtr compileAction(std::string actionString, RulePtr rule,
               LatticeWrapperPtr latticeWrapper);

        std::string compileRulePattern(std::string &matched, int &size,
                std::vector<std::string> &tokensPatterns,
                std::vector<std::string> &tokensModifiers,
                std::vector<bool> &tokensRequired,
                std::vector<int> &matchedIndices, int &bracketCount,
                NegativePatternStrings &negativePatterns);

    private:
        TagsetPtr tagset;
        int numAttributes;
        std::string compileNonTokens(std::string &matched);
        std::string compileRuleName(std::string matched);
        //std::string compileRuleMatch(std::string &matched, int &size, std::vector<string> &tokensPatterns, std::vector<string> &tokensModifiers, std::vector<bool> &tokensRequired, std::vector<int> &matchedIndexes, int &bracketCount);
        ActionsPtr compileRuleAction(std::string &matched, int ruleLeftCount,
                int ruleMatchCount, int ruleRightCount, std::string ruleName,
                bool &repeat, LatticeWrapperPtr latticeWrapper);

        int countTokens(std::string &matched);
        std::string getToken(std::string &matched, std::string &before);
        std::string compileToken(std::string &token,
                NegativePatternStrings &negativePatterns, bool no_prefix = false);
        std::string getKey(std::string &token);
        std::string getValue(std::string &token);
        std::string getOperator(std::string &token);

        //bool compilePosCondition(std::string &comparisonOperator, std::string &value, Interpretations &interps);
        bool compilePosCondition(std::string &comparisonOperator,
                std::string &value, TokenPatterns &tokenPatterns);
        //bool compileBaseCondition(std::string &comparisonOperator, std::string &value, Interpretations &interps, bool icase);
        bool compileBaseCondition(std::string &comparisonOperator,
                std::string &value, TokenPatterns &tokenPatterns, bool icase);
        //bool compileAttributeCondition(std::string &key, std::string &comparisonOperator, std::string &value, Interpretations &interps);
        bool compileAttributeCondition(std::string &key,
                std::string &comparisonOperator, std::string &value,
                TokenPatterns &tokenPatterns);
        std::string compileOrthCondition(std::string &comparisonOperator, std::string &value, bool icase);
        //std::string interpretationsToString(Interpretations interps);
        std::string generateTokenPatternsString(TokenPatterns tokenPatterns,
                NegativePatternStrings &negativePatterns);
        //bool compileDeleteCondition(std::string &key, std::string &comparisonOperator, std::string &value, TokenMask &mask, std::string &orth);
        bool compileDeleteCondition(std::string &key, std::string &comparisonOperator,
                std::string &value, DeleteConditions &conditions);
        //bool compileAddInterpretation(std::string &pattern, std::vector<InterpretationPair> &interpretations);
        bool compileAddInterpretation(std::string &pattern, std::vector<std::string> &interpretations);

        //TokenMask baseMask;
        //int baseIndex;
        //int modifierIndex;

        //std::string nothingSet;
        //boost::u32regex regNothingSet;
        //PatternPtr regNothingSet;

        bool verbose;
        bool syntok;

        RulesPtr rules;
//        RulesPtr rules;
        //std::string trim(std::string s);
       // std::string baseMask;

//        boost::regex regAmp;
//        boost::regex regLt;
//        boost::regex regGt;
//        boost::regex regLPar;
//        boost::regex regRPar;
//        boost::regex regAlt, regPlus, regAsterisk, regOpt;
        PatternPtr regAmp;
        PatternPtr regLt;
        PatternPtr regGt;
        PatternPtr regLPar;
        PatternPtr regRPar;
        PatternPtr regAlt, regPlus, regAsterisk, regOpt;

        void initProperties();
        void initPatterns();
};

}

}

}

#endif

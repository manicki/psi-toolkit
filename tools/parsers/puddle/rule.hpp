#ifndef RULE_H__
#define RULE_H__

#include <vector>
#include <string>
#include "regexp.hpp"
#include "action.hpp"
#include "group_action.hpp"
#include "delete_action.hpp"
#include "add_action.hpp"
#include "unify_action.hpp"
#include "syntok_action.hpp"
#include "join_action.hpp"
#include "attach_action.hpp"
#include "transform_action.hpp"
#include <sstream>
#include <boost/shared_ptr.hpp>

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Rule
{
    public:
#if HAVE_RE2
        Rule(std::string aName, std::string aCompiled, int aLeftCount,
                int aMatchCount, int aRightCount, ActionsPtr aActions,
                RuleTokenPatterns aRuleTokenPatterns,
                RuleTokenModifiers aRuleTokenModifiers,
                RuleTokenRequirements aRuleTokenRequirements,
                RulePatternIndices aRulePatternIndices, bool aRepeat,
                std::string aLeft, std::string aMatch, std::string aRight,
                NegativePatternStrings aNegativePatterns);
#else
        Rule(std::string aName, std::string aCompiled, int aLeftCount,
                int aMatchCount, int aRightCount, ActionsPtr aActions,
                RuleTokenPatterns aRuleTokenPatterns,
                RuleTokenModifiers aRuleTokenModifiers,
                RuleTokenRequirements aRuleTokenRequirements,
                RulePatternIndices aRulePatternIndices, bool aRepeat,
                std::string aLeft, std::string aMatch, std::string aRight);
#endif

        int matchPattern(std::string &sentenceString,
                int &afterIndex, std::vector<StringPiece> &match);
        bool test(std::string &sentenceString, Lattice &lattice,
                int matchedStartIndex, std::vector<StringPiece> &match,
                RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);
        bool apply(std::string &sentenceString, Lattice &lattice,
                int matchedStartIndex, RuleTokenSizes &ruleTokenSizes,
                std::list<Lattice::EdgeSequence> &rulePartitions);

        std::string getName() const;
        int getLeftCount() const;
        int getMatchCount() const;
        int getRightCount() const;
        bool getRepeat() const;

        void setPattern(std::string aCompiled);
        void setLeftCount(int aCount);
        void setMatchCount(int aCount);
        void setRightCount(int aCount);
        void setRepeat(bool aRepeat);

        void setMatch(std::string aMatch);
        void setLeft(std::string aLeft);
        void setRight(std::string aRight);
        std::string getMatch() const;
        std::string getLeft() const;
        std::string getRight() const;

        void setRuleTokenPatterns(RuleTokenPatterns aRuleTokenPatterns);
        void setRuleTokenModifiers(RuleTokenModifiers aRuleTokenModifiers);
        void setRuleTokenRequirements(RuleTokenRequirements aRuleTokenRequirements);
        void setRulePatternIndices(RulePatternIndices aRulePatternIndices);

        void addAction(ActionPtr action);
        void deleteAction(size_t index);

    private:
        std::string name;
        PatternPtr pattern;
#if HAVE_RE2
        NegativePatterns negativePatterns;
#endif
        std::string compiled;
        std::string left_, match_, right_;
        ActionsPtr actions;
        int leftCount, matchCount, rightCount;
        std::string beforeMatch;
        int countTokensMatched(std::string matched);

        RuleTokenPatterns ruleTokenPatterns;
        RuleTokenModifiers ruleTokenModifiers;
        RuleTokenRequirements ruleTokenRequirements;
        RulePatternIndices rulePatternIndices;

        bool repeat;

        int getPatternStart(std::string &pattern);
        int getPatternEnd(std::string &pattern);

        bool requiredTokensMatched(std::vector<StringPiece> &match,
                RuleTokenSizes &ruleTokenSizes);
};

typedef boost::shared_ptr<Rule> RulePtr;
typedef std::vector<RulePtr> Rules;
typedef boost::shared_ptr<Rules> RulesPtr;

}

}

}

#endif

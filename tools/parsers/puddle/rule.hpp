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

#if HAVE_RE2
    typedef std::map<std::string, std::string> NegativePatternStrings;
    typedef std::map<std::string, PatternPtr> NegativePatterns;
#endif

class Rule
{
    public:
        Rule();
#if HAVE_RE2
        Rule(std::string aName, std::string aCompiled, int aLeftCount,
                int aMatchCount, int aRightCount, ActionsPtr aActions,
                std::vector<std::string> aTokensPatterns,
                std::vector<std::string> aTokensModifiers,
                std::vector<bool> aTokensRequired,
                std::vector<int> aMatchedIndices, bool aRepeat,
                std::string aLeft, std::string aMatch, std::string aRight,
                NegativePatternStrings aNegativePatterns); //@todo: zmienic tu typy, bo mnie krew zaleje
#else
        Rule(std::string aName, std::string aCompiled, int aLeftCount,
                int aMatchCount, int aRightCount, ActionsPtr aActions,
                std::vector<std::string> aTokensPatterns,
                std::vector<std::string> aTokensModifiers,
                std::vector<bool> aTokensRequired,
                std::vector<int> aMatchedIndices, bool aRepeat,
                std::string aLeft, std::string aMatch, std::string aRight);
#endif
        ~Rule();

        //bool test(std::string &sentence, Entities &entities, int currentEntity);
        //bool test(std::string &sentenceString, ParseGraphPtr pg, int currentEntity);
        bool test(std::string &sentenceString, Lattice &lattice,
                int currentEntity, std::vector<StringPiece> &match,
                RuleTokenSizes &ruleTokenSizes);
        //bool apply(std::string &sentence, Entities &entities, Edges &edges, int currentEntity);
        //bool apply(std::string &sentenceString, ParseGraphPtr pg, Lattice &lattice, int currentEntity);
        bool apply(std::string &sentenceString, Lattice &lattice,
                int currentEntity, RuleTokenSizes &ruleTokenSizes);

        std::string getName() const;
        PatternPtr getPattern() const;
        int getLeftCount() const;
        int getMatchCount() const;
        int getRightCount() const;
        std::string getCompiled() const;
        ActionsPtr getActions() const;
        bool getRepeat() const;

        void setName(std::string aName);
        void setPattern(std::string aCompiled);
        void setLeftCount(int aCount);
        void setMatchCount(int aCount);
        void setRightCount(int aCount);
        void setActions(Actions aActions);
        void setRepeat(bool aRepeat);

        void setMatch(std::string aMatch);
        void setLeft(std::string aLeft);
        void setRight(std::string aRight);
        std::string getMatch() const;
        std::string getLeft() const;
        std::string getRight() const;

        void setTokensPatterns(std::vector<std::string> aTokensPatterns);
        void setTokensModifiers(std::vector<std::string> aTokensModifiers);
        void setTokensRequired(std::vector<bool> aTokensRequired);
        void setMatchedIndices(std::vector<int> aMatchedIndices);

        int matchPattern(std::string &sentence,
                std::string &beforeMatched, std::string &afterMatched,
                int &afterIndex, std::vector<StringPiece> &match);

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
        int countEntities(std::string matched);

        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedIndices;

        bool repeat;

        int getPatternStart(std::string &pattern); //wcale nie powiedziane, ze to bedzie wyciagac (tylko lub w ogole) start, wiec funkcja z nazwa wlacznie ulegac ma zmianom
        int getPatternEnd(std::string &pattern); //wcale nie powiedziane, ze to bedzie wyciagac (tylko lub w ogole) start, wiec funkcja z nazwa wlacznie ulegac ma zmianom
};

typedef boost::shared_ptr<Rule> RulePtr;
typedef std::vector<RulePtr> Rules;
typedef boost::shared_ptr<Rules> RulesPtr;

}

}

}

#endif

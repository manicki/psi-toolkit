#ifndef RULE_H__
#define RULE_H__

#include <vector>
#include <string>

//#include <boost/regex.hpp>
//#include <boost/regex/icu.hpp>

//#include "utf8_converter.hpp"

#include "TransitionInfo.hpp"
#include "entity.hpp"
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
        Rule();
        Rule(std::string aName, std::string aCompiled, int aLeftCount, int aMatchCount, int aRightCount, ActionsPtr aActions, std::vector<std::string> aTokensPatterns, std::vector<std::string> aTokensModifiers, std::vector<bool> aTokensRequired, std::vector<int> aMatchedIndexes, bool aRepeat, std::string aLeft, std::string aMatch, std::string aRight); //@todo: zmienic tu typy, bo mnie krew zaleje
        ~Rule();

        bool test(std::string &sentence, Entities &entities, int currentEntity);
        bool apply(std::string &sentence, Entities &entities, Edges &edges, int currentEntity);

        std::string getName();
        PatternPtr getPattern();
        int getLeftCount();
        int getMatchCount();
        int getRightCount();
        std::string getCompiled();
        ActionsPtr getActions();
        bool getRepeat();

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
        std::string getMatch();
        std::string getLeft();
        std::string getRight();

        void setTokensPatterns(std::vector<std::string> aTokensPatterns);
        void setTokensModifiers(std::vector<std::string> aTokensModifiers);
        void setTokensRequired(std::vector<bool> aTokensRequired);
        void setMatchedIndexes(std::vector<int> aMatchedIndexes);

        int matchPattern(std::string &sentence, int matchNumber, std::string &beforeMatched);

        void addAction(ActionPtr action);
        void deleteAction(int index);

        std::string makeReadable();
        std::string log();

    private:
        std::string name;
        std::vector<TransitionInfo> elements;
        PatternPtr pattern;
        std::string compiled;
        std::string left_, match_, right_;
        ActionsPtr actions;
        int leftCount, matchCount, rightCount;
        std::string beforeMatch;
        int countEntities(std::string matched);

        //boost::match_results<std::string::const_iterator> match;
        re2::StringPiece *match;
        bool match_set; //note: to jest nowosc, przy zmianie biblioteki wyr. reg. - moze nie byc to konieczne koniec koncow
        std::string matching;

        std::vector<std::string> tokensPatterns;
        std::vector<std::string> tokensModifiers;
        std::vector<bool> tokensRequired;
        std::vector<int> matchedTokensSize;
        std::vector<int> matchedIndexes;

        bool repeat;

        int getPatternStart(std::string &matchedPattern); //wcale nie powiedziane, ze to bedzie wyciagac (tylko lub w ogole) start, wiec funkcja z nazwa wlacznie ulegac ma zmianom
};

typedef boost::shared_ptr<Rule> RulePtr;
typedef std::vector<RulePtr> Rules;
typedef boost::shared_ptr<Rules> RulesPtr;

}

}

}

#endif

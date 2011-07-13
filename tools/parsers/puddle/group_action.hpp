#ifndef GROUPACTION_H__
#define GROUPACTION_H__

#include "action.hpp"
#include "group.hpp"
#include "token.hpp"
#include "syntok.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class GroupAction : public Action
{
    public:
        GroupAction(std::string aGroup, int aStart, int aEnd, int aHead, std::string aRuleName);
        ~GroupAction();
        bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        std::string getGroup();
        int getHead();
        void setGroup(std::string aGroup);
        void setHead(int aHead);
        int getStart();
        void setStart(int aStart);
        int getEnd();
        void setEnd(int aEnd);
        std::string getRuleName();
        void setRuleName(std::string aRuleName);

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        std::string group;
        int head;
        int start, end;
        std::string ruleName;
        std::string type;
        bool verbose;
};

typedef boost::shared_ptr<GroupAction> GroupActionPtr;

}

}

}
//typedef std::vector<GroupAction> Actions;

#endif

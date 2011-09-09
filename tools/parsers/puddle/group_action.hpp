#ifndef GROUPACTION_H__
#define GROUPACTION_H__

#include "action.hpp"
//#include "group.hpp"
//#include "token.hpp"
//#include "syntok.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class GroupAction : public Action
{
    public:
        GroupAction(std::string aGroup, unsigned int aStart, unsigned int aEnd, unsigned int aHead,
                std::string aRuleName);
        ~GroupAction();
        //bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        //bool apply(ParseGraphPtr pg, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool apply(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);
        //bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        //bool test(ParseGraphPtr pg, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(Lattice &lattice, int,
                RuleTokenSizes &TokenSizes);
        std::string getGroup() const;
        unsigned int getHead() const;
        void setGroup(std::string aGroup);
        void setHead(unsigned int aHead);
        unsigned int getStart() const;
        void setStart(unsigned int aStart);
        unsigned int getEnd() const;
        void setEnd(unsigned int aEnd);
        std::string getRuleName() const;
        void setRuleName(std::string aRuleName);

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        std::string group;
        unsigned int head;
        unsigned int start, end;
        std::string ruleName;
        std::string type;
        bool verbose;
//        LatticeWrapperPtr latticeWrapper;
};

typedef boost::shared_ptr<GroupAction> GroupActionPtr;

}

}

}
//typedef std::vector<GroupAction> Actions;

#endif

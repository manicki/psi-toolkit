#ifndef JOINACTION_H__
#define JOINACTION_H__

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

class JoinAction : public Action
{
    public:
        JoinAction(std::string aGroup, int aStart, int aEnd, int aHead, std::string aRuleName);
        ~JoinAction();
        //bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        //bool apply(ParseGraphPtr, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool apply(Lattice &lattice, int currentEntity,
                std::vector<int> matchedTokensSize);
        //bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        //bool test(ParseGraphPtr, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(Lattice &lattice, int,
                std::vector<int> matchedTokensSize);
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
        size_t head;
        size_t start, end;
        std::string ruleName;
        std::string type;
        bool verbose;
//        LatticeWrapperPtr latticeWrapper;
};

typedef boost::shared_ptr<JoinAction> JoinActionPtr;

}

}

}

#endif

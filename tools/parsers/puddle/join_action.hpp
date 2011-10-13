#ifndef JOINACTION_H__
#define JOINACTION_H__

#include "action.hpp"

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
        bool apply(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);
        bool test(Lattice &lattice, int,
                RuleTokenSizes &ruleTokenSizes);
        std::string getGroup() const;
        int getHead() const;
        void setGroup(std::string aGroup);
        void setHead(int aHead);
//        int getStart() const;
        void setStart(int aStart);
//        int getEnd() const;
        void setEnd(int aEnd);
        std::string getRuleName() const;
        void setRuleName(std::string aRuleName);

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        void init(std::string aGroup, int aStart, int aEnd, int aHead,
                std::string aRuleName);
        std::string group;
        size_t head;
        size_t start, end;
        std::string ruleName;
        std::string type;
        bool verbose;
};

typedef boost::shared_ptr<JoinAction> JoinActionPtr;

}

}

}

#endif

#ifndef GROUPACTION_H__
#define GROUPACTION_H__

#include "action.hpp"

namespace poleng
{

    namespace bonsai
    {

        namespace puddle
        {

            class GroupAction : public Action
            {
                public:
                    GroupAction(std::string aGroup, unsigned int aStart,
                            unsigned int aEnd, unsigned int aHead,
                            std::string aRuleName);
                    bool apply(Lattice &lattice, int currentEntity,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);
                    bool test(Lattice &lattice, int currentEntity,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);
//                    bool apply(Lattice &lattice, int currentEntity,
//                            RuleTokenSizes &ruleTokenSizes);
//                    bool test(Lattice &lattice, int,
//                            RuleTokenSizes &TokenSizes);

                private:
                    void init(std::string aGroup, unsigned int aStart,
                            unsigned int aEnd, unsigned int aHead,
                            std::string aRuleName);
                    std::string group;
                    unsigned int head;
                    unsigned int start, end;
                    std::string ruleName;
            };

            typedef boost::shared_ptr<GroupAction> GroupActionPtr;

        }

    }

}

#endif

#ifndef GROUPACTION_HDR
#define GROUPACTION_HDR

#include "action.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class GroupAction : public Action {
                public:
                    GroupAction(std::string aGroup, unsigned int aStart,
                            unsigned int aEnd, unsigned int aHead,
                            std::string aRuleName);
                    bool apply(Lattice &lattice, std::string langCode,
                            int matchedStartIndex,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);
                    bool test(Lattice &lattice, std::string langCode,
                            int matchedStartIndex,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);

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

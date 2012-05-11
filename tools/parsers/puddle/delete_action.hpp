#ifndef DELETEACTION_HDR
#define DELETEACTION_HDR

#include "action.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class DeleteAction : public Action {
                public:
                    DeleteAction(DeleteConditions aConditions, int aTokenIndex);
                    bool apply(Lattice &lattice, std::string langCode,
                            int matchedStartIndex,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);
                    bool test(Lattice &lattice, std::string langCode,
                            int matchedStartIndex,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);

                private:
                    void init(DeleteConditions aConditions, int aTokenIndex);
                    bool foundEdgesToDelete(Lattice &lattice, std::string langCode,
                            Lattice::VertexDescriptor vertex, int count);
                    int tokenIndex;
                    DeleteConditions conditions;
            };

            typedef boost::shared_ptr<DeleteAction> DeleteActionPtr;
        }

    }

}

#endif

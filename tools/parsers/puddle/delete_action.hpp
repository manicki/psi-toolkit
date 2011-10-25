#ifndef DELETEACTION_H__
#define DELETEACTION_H__

#include "action.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class DeleteAction : public Action {
                public:
                    DeleteAction(DeleteConditions aConditions, int aTokenIndex);
                    bool apply(Lattice &lattice, int currentEntity,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);
                    bool test(Lattice &lattice, int currentEntity,
                            RuleTokenSizes &ruleTokenSizes,
                            std::list<Lattice::EdgeSequence> &rulePartitions);
//                    bool apply(Lattice &lattice, int currentEntity,
//                            RuleTokenSizes &ruleTokenSizes);
//                    bool test(Lattice &lattice, int currentEntity,
//                            RuleTokenSizes &ruleTokenSizes);

                private:
                    void init(DeleteConditions aConditions, int aTokenIndex);
                    bool foundEdgesToDelete(Lattice &lattice,
                            Lattice::VertexDescriptor vertex, int count);
                    int tokenIndex;
                    DeleteConditions conditions;
            };

            typedef boost::shared_ptr<DeleteAction> DeleteActionPtr;
        }

    }

}

#endif

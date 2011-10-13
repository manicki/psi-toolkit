#ifndef DELETEACTION_H__
#define DELETEACTION_H__

#include "action.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class DeleteAction : public Action {
                public:
                    DeleteAction(DeleteConditions aConditions, int aTokenIndex,
                            std::string uPattern);
                    bool apply(Lattice &lattice, int currentEntity,
                            RuleTokenSizes &ruleTokenSizes);
                    bool test(Lattice &lattice, int currentEntity,
                            RuleTokenSizes &ruleTokenSizes);
                    std::string getUPattern() const;
                    int getTokenIndex() const;
                    void setTokenIndex(int aTokenIndex);
                    DeleteConditions getConditions() const;

                    std::string getType() { return type; }
                    void setVerbose() { verbose = true; }

                private:
                    void init(DeleteConditions aConditions, int aTokenIndex,
                            std::string uPattern);
                    bool foundEdgesToDelete(Lattice &lattice,
                            Lattice::VertexDescriptor vertex, int count);
                    int tokenIndex;
                    std::string type;
                    bool verbose;
                    std::string pattern_;
                    DeleteConditions conditions;
            };

            typedef boost::shared_ptr<DeleteAction> DeleteActionPtr;
        }

    }

}

#endif

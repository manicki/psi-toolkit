#ifndef DELETEACTION_H__
#define DELETEACTION_H__

#include "action.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            enum DeleteConditionType {
                //ORTH_CONDITION, - nie ma chyba warunkow delete orth!~ bo bez sensu
                BASE_CONDITION,
                MORPHOLOGY_CONDITION
            };

            class DeleteCondition {
                public:
                    DeleteConditionType type;
                    bool negation;
                    std::string pattern;
            };
            typedef std::vector<DeleteCondition> DeleteConditions;

            class DeleteAction : public Action {
                public:
                    //DeleteAction(std::string aPattern, int aTokenIndex, std::string uPattern);
                    DeleteAction(DeleteConditions aConditions, int aTokenIndex,
                            std::string uPattern, LatticeWrapperPtr aLatticeWrapper);
                    //bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
                    bool apply(ParseGraphPtr pg, Lattice &lattice,
                            int currentEntity,
                            std::vector<int> matchedTokensSize);
                    //bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
                    bool test(ParseGraphPtr pg, Lattice &lattice,
                            int currentEntity,
                            std::vector<int> matchedTokensSize);
                    //void setPattern(std::string aPattern);
                    //std::string getPattern();
                    std::string getUPattern();
                    int getTokenIndex();
                    void setTokenIndex(int aTokenIndex);
                    DeleteConditions getConditions();

                    std::string getType() { return type; }
                    void setVerbose() { verbose = true; }

                private:
                    //PatternPtr pattern;
                    //std::string patternString;
                    int tokenIndex;
                    //bool nothingToDelete;
                    std::string type;
                    bool verbose;
                    std::string pattern_;
                    DeleteConditions conditions;
                    LatticeWrapperPtr latticeWrapper;
            };

            typedef boost::shared_ptr<DeleteAction> DeleteActionPtr;
        }

    }

}

#endif

#ifndef DELETEACTION_H__
#define DELETEACTION_H__

#include "action.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class DeleteAction : public Action {
                public:
                    DeleteAction(std::string aPattern, int aTokenIndex, std::string uPattern);
                    bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
                    bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
                    void setPattern(std::string aPattern);
                    std::string getPattern();
                    std::string getUPattern();
                    int getTokenIndex();
                    void setTokenIndex(int aTokenIndex);

                    std::string getType() { return type;}
                    void setVerbose() { verbose = true; }

                private:
                    PatternPtr pattern;
                    std::string patternString;
                    int tokenIndex;
                    bool nothingToDelete;
                    std::string type;
                    bool verbose;
                    std::string pattern_;
            };

            typedef boost::shared_ptr<DeleteAction> DeleteActionPtr;
        }

    }

}

#endif

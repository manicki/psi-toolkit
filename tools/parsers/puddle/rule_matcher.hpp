#ifndef RULEMATCHER_H__
#define RULEMATCHER_H__

#include <string>
#include <vector>

#include "ParseGraph.hpp"
#include "TransitionInfo.hpp"

#include "rule.hpp"
#include "entity.hpp"
#include "token.hpp"
#include "group.hpp"
#include <boost/algorithm/string.hpp>

#include "puddle_util.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class RuleMatcher {
                public:
                    RuleMatcher();
                    ~RuleMatcher();
                    ParseGraphPtr applyRules(std::string &sentence, Entities &entities, Edges &lattice);

                    //void setRules(Rules aRules);
                    void setRules(RulesPtr aRules);

                    void setSyntok();
                    void setNoSyntok();
                    void setDisamb();
                    void setNoRepeats();

                private:
                    RulesPtr rules;
                    bool disamb; //@todo: do czego to jest?
                    bool norepeats;
                    bool syntok; //@todo: do czego to jest?

                    void unescapeSpecialChars(Edges &edges);
                    void addPosEdges(Edges &edges);
            };

        }

    }

}

#endif


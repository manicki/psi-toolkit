#ifndef RULEMATCHER_H__
#define RULEMATCHER_H__

#include <string>
#include <vector>

//#include "TransitionGraph.hpp"
#include "ParseGraph.hpp"
#include "TransitionInfo.hpp"

#include "rule.hpp"
//#include "TransitionGraph.hpp"
#include "entity.hpp"
#include "token.hpp"
#include "group.hpp"
//#include "XmlWriter.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
//#include "icu_converter.hpp"
#include <boost/algorithm/string.hpp>
//#include <unicode/schriter.h>

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
                    //        void setXmlWriter(XmlWriter *aXmlWriter);

                    static int sentencesCount; //@todo: potrzebne to? wywalic to w diably
                    static boost::posix_time::ptime start_time; //@todo: potrzebne to? wywalic to w diably

                    void setSyntok();
                    void setNoSyntok();
                    //        void setNoxml();
                    void setDisamb();
                    void setNoRepeats();

                private:
                    RulesPtr rules;
                    bool disamb; //@todo: do czego to jest?
                    bool norepeats;
                    //        XmlWriter *xmlWriter;
                    bool syntok; //@todo: do czego to jest?
                    //        bool noxml;
            };

        }

    }

}

#endif


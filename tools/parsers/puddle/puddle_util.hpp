#ifndef PUDDLE_UTIL_H__
#define PUDDLE_UTIL_H__

#include <string>
//#include <re2/re2.h>

#include <sstream>
//#include "ParseGraph.hpp"
#include "puddle_types.hpp"

#include <boost/algorithm/string/join.hpp>

namespace poleng {
    namespace bonsai {
        namespace puddle {
            namespace util {

                std::string escapeSpecialChars(std::string &s);
                std::string unescapeSpecialChars(std::string &s);

                std::string getMorphologyString(Morphology morphology);
//                TransitionInfo* getEdge(ParseGraphPtr pg, int index);
//                TransitionInfo* getEdge(ParseGraphPtr pg, int index, int offset);
//
//                std::string getNewEdgeId(ParseGraphPtr pg);
//                void removeGraphEdge(ParseGraphPtr pg, TransitionInfo edge);
            }
        }
    }
}


#endif

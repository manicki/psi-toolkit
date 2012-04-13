#ifndef PUDDLE_TYPES_HDR
#define PUDDLE_TYPES_HDR

#include "regexp.hpp"
#include "utf8/utf8.h"
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

namespace poleng {

    namespace bonsai {

        namespace puddle {

            typedef std::vector<int> RuleTokenSizes;
            typedef std::vector<std::string> RuleTokenPatterns;
            typedef std::vector<std::string> RuleTokenModifiers;
            typedef std::vector<bool> RuleTokenRequirements;
            typedef std::vector<int> RulePatternIndices;

            typedef RegExp Pattern;
            typedef boost::shared_ptr<Pattern> PatternPtr;
#if HAVE_RE2
            typedef std::map<std::string, std::string> NegativePatternStrings;
            typedef std::map<std::string, PatternPtr> NegativePatterns;
#endif

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

            typedef std::map<std::string, std::string> Morphology;

            typedef utf8::iterator<std::string::iterator> utf8iterator;
            //@todo: zmienic to na utf8::unchecked::iterator w wersji "produkcyjnej"?
            //gdzie bedzie wowczas sprawdzenie poprawnosci utf8 wejsciowego?


        }

    }

}

#endif

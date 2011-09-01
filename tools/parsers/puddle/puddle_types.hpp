#ifndef PUDDLE_TYPES_H__
#define PUDDLE_TYPES_H__

//#include <re2/re2.h>
#include "regexp.hpp"
#include "utf8.h"
#include <string>
#include <boost/shared_ptr.hpp>
//#include "ParseGraph.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            typedef RegExp Pattern;
            typedef boost::shared_ptr<Pattern> PatternPtr;

            typedef utf8::iterator<std::string::iterator> utf8iterator; //@todo: zmienic to na utf8::unchecked::iterator w wersji "produkcyjnej"? gdzie bedzie wowczas sprawdzenie poprawnosci utf8 wejsciowego?

        }

    }

}

#endif

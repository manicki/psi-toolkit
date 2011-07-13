#ifndef METAGGER_H__
#define METAGGER_H__

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <fstream>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>

#include "tagger.hpp"
#include "entity.hpp"
#include "token.hpp"

//#include "maxentmodel.hpp"

namespace poleng {

namespace bonsai {
    namespace puddle {
    
        class MeTagger : public Tagger {
          public:
            MeTagger();
            std::string tagSentence(std::string, Entities &, Edges &);
            std::string tagSentenceITF(std::string, Entities &, Edges &);
            std::string tag(std::string, Entities &, Edges &);
            void add_sample_sentence(std::string);
            void train(int iterations=50);
            void saveModel(std::string);
            void loadModel(std::string);
            
          private:
            typedef std::string Outcome;
            typedef std::string Feature;
            typedef std::vector<Feature> Context;
            
            boost::u32regex rxUpperCaseFirst;
            boost::u32regex rxUpperCaseAll;
            boost::u32regex rxContainsNumber;
            boost::u32regex rxIsNumber;
            boost::u32regex rxContainsPunct;
            boost::u32regex rxContainsLetter;
            
            maxent::MaxentModel m;
            bool openForEvents;
            bool posModelLoaded;
            
            boost::u32regex tag_to_regex(std::string);
            Context create_context(Entities &, int, int);
        };
    
        typedef boost::shared_ptr<MeTagger> MeTaggerPtr;
    }
}

}

#endif

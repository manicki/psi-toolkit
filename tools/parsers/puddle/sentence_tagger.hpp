#ifndef TAGGER_H__
#define TAGGER_H__

#include <string>
#include <sstream>
#include "puddle.hpp"
#include "tagset_loader.hpp"
//#include "../parser/ParseConverter.hpp"

namespace poleng
{
    
namespace bonsai
{

class Tagger
{
    private:
        puddle::PuddlePtr parser;
//        ParseConverter converter;

    public:
        Tagger(puddle::PuddlePtr parser_);
        //Tagger(std::string tagset_file, std::string desc_file, std::string rules_file, std::string dict_file);
        ~Tagger();
        std::string tagSentence(std::string sentence);
};

}

}

#endif


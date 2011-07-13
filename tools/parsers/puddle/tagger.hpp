#ifndef PUDDLETAGGER_H__
#define PUDDLETAGGER_H__

#include "TransitionInfo.hpp"
#include "entity.hpp"
#include "token.hpp"
#include "group.hpp"
#include "syntok.hpp"
#include "special_token.hpp"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "tagset.hpp"
#include "icu_converter.hpp"
#include <unicode/schriter.h>
#include <boost/shared_ptr.hpp>

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Tagger
{
    public:
        Tagger();
        ~Tagger();
        void loadDictionary(std::string &filename);
        std::string tagSentence(std::string sentence, Entities &entities, Edges &edges);
        std::string getReadable(Entities entities);
        std::string tagSentenceReadable(std::string sentence, Entities &entities, Edges &edges);
        std::string processInput(std::string &sentence, Entities &entities, Edges &edges);

        void setTagset(TagsetPtr aTagset);

    protected:
        std::map<std::string, std::string> *data;

        TagsetPtr tagset;
        std::string emptyMorphology;
        boost::regex regSeparator;
        boost::regex regPunct;
        boost::regex regDigit;
        boost::regex regInterpSeparator;
//        boost::regex regEscAmp;
//        boost::regex regEscLt;
//        boost::regex regEscGt;
        boost::regex regLt;
        boost::regex regAmp;
        boost::regex regGt;
        boost::regex regLPar;
        boost::regex regRPar;
        boost::regex regAlt, regPlus, regAsterisk, regOpt;

        boost::regex regSep;
//        boost::regex regEscLPar;
//        boost::regex regEscRPar;
};

typedef boost::shared_ptr<Tagger> TaggerPtr;

}

}

}

#endif

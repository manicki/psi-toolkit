#ifndef TAGSET_H__
#define TAGSET_H__

#include <string>
#include <map>
#include <vector>
#include <set>

#include <boost/shared_ptr.hpp>
#include "puddle_types.hpp"

//using namespace puddle;

#define DESC_ORTH 0
#define DESC_BASE 1
#define DESC_MORPHO 2

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Tagset
{
    public:
        void readFromFile(std::string &filename);
        bool checkAttribute(std::string &attribute);
        bool checkAttribute(std::string &pos, std::string &attribute);
        bool checkPos(std::string &pos);
        bool checkAttributeValue(std::string &attribute, std::string &value);
        Tagset();
        Tagset(std::string &filename);
        ~Tagset();
        bool checkMorphology(std::string &morphology);
        int getNumberOfAttributes();
        int getAttributeIndex(std::string &attribute);
        size_t size();
        std::string getAttributeAtIndex(std::string &pos, int index);
        std::vector<std::string> getAttributeValues(std::string &attribute);

#if _WITH_BONSAI_PARSEGRAPH
        void readDescFromFile(std::string &filename);
        bool containsDesc();
        bool desc_terminals;
        bool desc_nonterminals;
        std::vector<int> desc_order;
        std::string desc_separator;
        std::vector<std::string> getGroupDesc(std::string group);
        std::vector<std::string> getTokenDesc(std::string pos);
#endif
        std::map<std::string, std::string> getAttributes(std::string morphologyString, bool buffer = false);

        std::vector<std::string> getOpenClasses() const;

        std::vector<std::string> getPosMatching(std::string regexp);
        std::vector<std::string> getPosNotMatching(std::string regexp);
        std::vector<std::string> getPosExcept(std::string pos);
        std::vector<std::string> getAttributeValuesMatching(std::string attribute, std::string regexp);
        std::vector<std::string> getAttributeValuesNotMatching(std::string attribute, std::string regexp);
        std::vector<std::string> getAttributeValuesExcept(std::string attribute, std::string value);

        std::vector<std::string> getAttributes();
        std::vector<std::string> getPartsOfSpeech();

    private:

        void parseAttribute(std::string &attribute, std::string &valuesString);
        void parsePartOfSpeech(std::string &partOfSpeech,
                std::string &attributesString);
        void parseOpenClasses(std::string &openClassesString);
        void generateProperMorphologyLabels();

        std::vector<std::string> attrList;
        std::map<std::string, std::vector<std::string> > attributes;
        std::map<std::string, std::vector<std::string> > partsOfSpeech;

#if _WITH_BONSAI_PARSEGRAPH
        std::map<std::string, std::vector<std::string> > token_description;
        std::map<std::string, std::vector<std::string> > group_description;
#endif

        std::map<std::string, std::map<std::string, std::string> > attributes_buffer;
        std::set<std::string> properMorphologyLabels;

        std::vector<std::string> open_classes;
};

typedef boost::shared_ptr<Tagset> TagsetPtr;

}

}

}

#endif


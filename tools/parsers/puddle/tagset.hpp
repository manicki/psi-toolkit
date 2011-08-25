#ifndef TAGSET_H__
#define TAGSET_H__

#include <string>
#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

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
        std::string log();
        Tagset();
        Tagset(std::string &filename);
        ~Tagset();
        //destruktory tez
        std::string mapMorphology(std::string &morphology);
        std::string unmapMorphology(std::string &mapped);
        char mapPos(std::string &pos);
        std::pair<char, int> mapAttributeValue(std::string &attribute, std::string &value);
        int getNumberOfAttributes();
        std::vector<char> mapPosMatching(std::string &regexp);
        std::vector<char> mapAttributeValuesMatching(std::string &attribute, std::string &regexp);
        int getAttributeIndex(std::string &attribute);
        int size();
        std::string getAttributeAtIndex(std::string &pos, int index);
        std::vector<std::string> getAttributeValues(std::string &attribute);

        void readDescFromFile(std::string &filename);
        bool containsDesc();
        bool desc_terminals;
        bool desc_nonterminals;
//        bool desc_include_pos;
        std::vector<int> desc_order;
        std::string desc_separator;
        std::vector<std::string> getGroupDesc(std::string group);
        std::vector<std::string> getTokenDesc(std::string pos);
        std::map<std::string, std::string> getAttributes(std::string morphologyString, bool buffer = false);

        const std::vector<std::string> getOpenClasses();
        std::vector<std::string> getMappedMorphologies(std::string mapped_pos);

        std::vector<std::string> getPosMatching(std::string regexp);
        std::vector<std::string> getPosNotMatching(std::string regexp);
        std::vector<std::string> getPosExcept(std::string pos);
        std::vector<std::string> getAttributeValuesMatching(std::string attribute, std::string regexp);
        std::vector<std::string> getAttributeValuesNotMatching(std::string attribute, std::string regexp);
        std::vector<std::string> getAttributeValuesExcept(std::string attribute, std::string value);

        std::vector<std::string> getAttributes();
        std::vector<std::string> getPartsOfSpeech();

    private:

        std::vector<std::string> attrList;
        std::map<std::string, std::vector<std::string> > attributes;
        std::map<std::string, std::map<std::string, char> > attrMappings;
        std::map<std::string, std::vector<std::string> > POSs;
        std::map<std::string, char> posMappings;

        std::map<std::string, std::string> morphologyMappings;

        std::map<std::string, std::vector<std::string> > token_description;
        std::map<std::string, std::vector<std::string> > group_description;

        std::map<std::string, std::map<std::string, std::string> > attributes_buffer;
        std::map<std::string, std::vector<std::string> > mapped_morphologies_by_pos; // zawiera wszystkie morfologie danej czesci mowy w postaci 'nieczytelnej'

        std::vector<std::string> open_classes;

//        bool verbose;
};

typedef boost::shared_ptr<Tagset> TagsetPtr;

}

}

}

#endif


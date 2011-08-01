#ifndef PARSECONVERTER_H__
#define PARSECONVERTER_H__

#include "ParseGraph.hpp"
#include <string>
#include "tagset.hpp"
//#include <boost/regex.hpp>
#include <re2/re2.h>

namespace poleng
{

namespace bonsai
{

class ParseConverter
{
    public:
        ParseConverter();
        ~ParseConverter();

        std::string convert(ParseGraph g);

        puddle::TagsetPtr tagset;
        void addDescription(ParseGraphPtr pg);
        void removeDescription(ParseGraphPtr pg);
        //void loadTagset(std::string tagset_filename, std::string description_filename);
        void setTagset(puddle::TagsetPtr tagset_);

    private:
};

}

}
#endif


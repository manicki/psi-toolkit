#ifndef UNIFYACTION_H__
#define UNIFYACTION_H__

#include <string>
#include <vector>
#include <set>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>

#include "utf8_converter.hpp"

#include "action.hpp"
#include "entity.hpp"
#include "token.hpp"
#include "group.hpp"
#include "syntok.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class UnifyAction : public Action
{
    public:
        UnifyAction(std::vector<int> aAttributeIndexes, std::vector<int> aTokenIndexes, std::vector<std::string> uAttributes);
        ~UnifyAction();
        bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);

        std::vector<int> getAttributeIndexes();
        void setAttributeIndexes(std::vector<int> aAttributeIndexes);
        std::vector<int> getTokenIndexes();
        void setTokenIndexes(std::vector<int> aTokenIndexes);

        std::vector<std::string> getUAttributes();

        void setBaseMask(std::string aBaseMask);

        std::string rule;

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        std::vector<int> attributeIndexes;
        std::vector<int> tokenIndexes;

        bool nullAgreement; // true = jak w czesci mowy atrybut nie wystepuje, to traktuj jako dajacy sie z unifikowac, false = jak w czesci mowy nie ma atrybutu, to nie da sie zunifikowac

        std::set<boost::regex> commonInterpretations;
        //std::set<boost::u32regex> commonInterpretations;

        std::string baseMask;
        int maskLength;

        std::string type;
        bool verbose;
        std::vector<std::string> attributes_;
};

typedef boost::shared_ptr<UnifyAction> UnifyActionPtr;

}

}

}

#endif

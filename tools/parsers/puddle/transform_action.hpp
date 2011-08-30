#ifndef TRANSFORMACTION_H__
#define TRANSFORMACTION_H__

#include "action.hpp"
//#include "group.hpp"
//#include "token.hpp"
//#include "syntok.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class TransformAction : public Action
{
    public:
        TransformAction(std::string aGroup, int aElement, std::string aRuleName);
        ~TransformAction();
        //bool apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize);
        //bool apply(ParseGraphPtr, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool apply(Lattice &lattice, int currentEntity,
                std::vector<int> matchedTokensSize);
        //bool test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize);
        //bool test(ParseGraphPtr, Lattice &lattice, int currentEntity, std::vector<int> matchedTokensSize);
        bool test(Lattice &lattice, int currentEntity,
                std::vector<int> matchedTokensSize);
        std::string getGroup() const;
        int getElement() const;
        void setGroup(std::string aGroup);
        void setElement(int aElement);
        std::string getRuleName() const;
        void setRuleName(std::string aRuleName);

        std::string getType() { return type;}

        void setVerbose() { verbose = true; }
    private:
        std::string group;
        int element;
        std::string ruleName;
        std::string type;
        bool verbose;
//        LatticeWrapperPtr latticeWrapper;
};

typedef boost::shared_ptr<TransformAction> TransformActionPtr;

}

}

}

//typedef std::vector<TransformAction> Actions;

#endif

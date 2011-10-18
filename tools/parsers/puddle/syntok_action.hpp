#ifndef SYNTOKACTION_H__
#define SYNTOKACTION_H__

#include "action.hpp"
#include "add_action.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

class SyntokAction : public Action
{
    public:
        SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndices,
                std::vector<Morphology> aMorphology, std::string aRuleName);
        ~SyntokAction();
        bool apply(Lattice &lattice, int currentEntity,
                RuleTokenSizes &ruleTokenSizes);
        bool test(Lattice&, int, RuleTokenSizes &ruleTokenSizes);

//        int getStart() const;
        void setStart(int aStart);
//        int getEnd() const;
        void setEnd(int aEnd);
        std::vector<Morphology> getMorphology() const;
        void setMorphology(std::vector<Morphology> aMorphology);
        std::vector<int> getTokenIndices() const;
        void setTokenIndices(std::vector<int> aTokenIndices);
        std::string getRuleName() const;
        void setRuleName(std::string aRuleName);

        std::string getType() { return type; }

        void setVerbose() { verbose = true; }
        void setSyntok() { syntok = true; }
    private:
        void init(int aStart, int aEnd, std::vector<int> aTokenIndices,
                std::vector<Morphology> aMorphology, std::string aRuleName);
        std::vector<std::string> generateBaseForms(Lattice &lattice,
                std::list<Lattice::EdgeSequence> edgeSequences);
        std::string generateOrth(Lattice &lattice,
                std::list<Lattice::EdgeSequence> edgeSequences);

        std::vector<int> tokenIndices;
        std::vector<Morphology> morphology;
        size_t start, end;

        std::string ruleName;
        std::string type;
        bool verbose;
        bool syntok;
};

typedef boost::shared_ptr<SyntokAction> SyntokActionPtr;

}

}

}

#endif



#include "group_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

GroupAction::GroupAction(std::string aGroup, unsigned int aStart,
        unsigned int aEnd, unsigned int aHead,
        std::string aRuleName) {
    init(aGroup, aStart, aEnd, aHead, aRuleName);
}

bool GroupAction::apply(Lattice &lattice, int matchedStartIndex,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence> &rulePartitions) {

    int realStart;
    int realEnd;
    int realHead;
    if (! util::getGroupActionParams(ruleTokenSizes, start, head, end,
            realStart, realHead, realEnd)) {
        throw PuddleRuleElementNotMatchedException(
                "Group rule head " + boost::lexical_cast<std::string>(head)
                + " is empty"
                );
    }

    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, realStart, matchedStartIndex);
    //Lattice::VertexDescriptor startVertex = matchedStartIndex + realStart; //@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    //Lattice::VertexDescriptor headVertex = lattice::getVertex(
    //        lattice, matchedStartIndex + realHead);
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, realHead, matchedStartIndex);
    //Lattice::VertexDescriptor headVertex = matchedStartIndex + realHead; //@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    //Lattice::VertexDescriptor endVertex = lattice::getVertex(
    //        lattice, matchedStartIndex + realEnd);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, realEnd, matchedStartIndex);
    //Lattice::VertexDescriptor endVertex = matchedStartIndex + realEnd; //@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = lattice::getTopEdges(
            lattice, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, endVertex);
    if (startEdges.empty() || headEdges.empty() || endEdges.empty()) {
        return false;
    }
    //std::list<Lattice::EdgeSequence> groupPartitions =
    //    lattice::getEdgesRange(
    //        lattice, startVertex, endVertex
    //        );
    //int headEdgeIndex = realHead - realStart
    lattice::addParseEdges(
            lattice,
            startEdges,
            endEdges,
            this->group,
            headEdges,
            rulePartitions,
            realHead
            );

    return true;
}

bool GroupAction::test(Lattice &lattice, int,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    if ( ( (size_t) lattice.getLastVertex() ) < head) {
        return false;
    }
    if (ruleTokenSizes[head - 1] == 0) {
        throw PuddleRuleElementNotMatchedException(
                "Group head element " + boost::lexical_cast<std::string>(head)
                + " is empty"
                );
    }
    return true;
}

void GroupAction::init(std::string aGroup, unsigned int aStart,
        unsigned int aEnd, unsigned int aHead,
        std::string aRuleName) {
    group = aGroup;
    start = aStart;
    end = aEnd;
    head = aHead;
    if ((head > (end+1)) || (head <= start)) {
        if (start == end)
            head = 0;
        else
            head = (head % (end+1)) - start;
    }
    type = "group";
    ruleName = aRuleName;
}


}

}

}


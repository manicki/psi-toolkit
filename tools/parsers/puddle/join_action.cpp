
#include "join_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

JoinAction::JoinAction(std::string aGroup, int aStart, int aEnd, int aHead,
        std::string aRuleName) {
    init(aGroup, aStart, aEnd, aHead, aRuleName);
}

bool JoinAction::apply(Lattice &lattice, int matchedStartIndex,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    int realStart;
    int realEnd;
    int realHead;
    if (! util::getGroupActionParams(ruleTokenSizes, start, head, end,
                realStart, realHead, realEnd))
        return false;

    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, realStart, matchedStartIndex);
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, realHead, matchedStartIndex);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, realEnd, matchedStartIndex);
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = lattice::getTopEdges(
            lattice, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, endVertex);
    lattice::removeParseEdges(lattice, startVertex, endVertex); //@todo: nie jestem przekonany, czy to jest dobre miejsce. addParseEdges moze sie wowczas nie powiesc. z drugiej strony, jak krawedzie nie sa usuniete tylko discarded, to moze sie nic nie stac. co tylko z groupPartitions? nie powinno byc generowane po usunieciu?
    std::list<Lattice::EdgeSequence> groupPartitions =
        lattice::getEdgesRange(
                lattice, startVertex, endVertex
                );
    lattice::addParseEdges(
            lattice,
            startEdges,
            endEdges,
            this->group,
            headEdges,
            groupPartitions,
            head - 1
            );
    return true;
}

bool JoinAction::test(Lattice &lattice, int,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    if ( ( (size_t) lattice.getLastVertex() ) < head ) {
        return false;
    }
    if (ruleTokenSizes[head - 1] == 0) {
//        std::cerr << "Element join: " << head - 1 << " empty!" << std::endl;
        return false;
    }
    return true;
}

void JoinAction::init(std::string aGroup, int aStart, int aEnd, int aHead,
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
    type = "join";
    ruleName = aRuleName;
}


}

}

}


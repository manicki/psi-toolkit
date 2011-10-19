
#include "attach_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

AttachAction::AttachAction(std::string aGroup, int aStart, int aEnd, int aHead,
        std::string aRuleName) {
    init(aGroup, aStart, aEnd, aHead, aRuleName);
}

bool AttachAction::apply(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {
    int realStart;
    int realEnd;
    int realHead;
    if (! util::getGroupActionParams(ruleTokenSizes, start, head, end,
                realStart, realHead, realEnd))
        return false;

    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, realStart, currentEntity);
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, realHead, currentEntity);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, realEnd, currentEntity);
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = lattice::getTopEdges(
            lattice, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, endVertex);
    lattice::removeParseEdges(lattice, headVertex, headVertex + 1);
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

bool AttachAction::test(Lattice &lattice, int,
        RuleTokenSizes &ruleTokenSizes) {
    if ( ( (size_t)lattice.getLastVertex() ) < head) {
        return false;
    }
    if (ruleTokenSizes[head - 1] == 0) {
//        std::cerr << "Element attach: " << head - 1 << " empty!" << std::endl;
        return false;
    }
    return true;
}

void AttachAction::init(std::string aGroup, int aStart, int aEnd, int aHead,
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
    type = "attach";
    ruleName = aRuleName;
}

}

}

}


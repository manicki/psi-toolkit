
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

AttachAction::~AttachAction() { }

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

std::string AttachAction::getGroup() const {
    return group;
}

int AttachAction::getHead() const {
    return head;
}

void AttachAction::setGroup(std::string aGroup) {
    group = aGroup;
}

void AttachAction::setHead(int aHead) {
    head = aHead;
}

//int AttachAction::getStart() const {
//    return start;
//}

void AttachAction::setStart(int aStart) {
    start = aStart;
}

//int AttachAction::getEnd() const {
//    return end;
//}

void AttachAction::setEnd(int aEnd) {
    end = aEnd;
}

std::string AttachAction::getRuleName() const {
    return ruleName;
}

void AttachAction::setRuleName(std::string aRuleName) {
    ruleName = aRuleName;
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
    verbose = false;
}

}

}

}


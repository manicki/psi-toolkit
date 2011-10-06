
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

GroupAction::~GroupAction() { } //@todo: wywalac te destruktory czy nie?

//@todo: zmienic chyba te nazwe currentEntity na cos wlasciwszego
bool GroupAction::apply(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {

    int realStart;
    int realEnd;
    int realHead;
    if (! util::getGroupActionParams(ruleTokenSizes, start, head, end,
            realStart, realHead, realEnd))
        return false;

    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, realStart, currentEntity);
    //Lattice::VertexDescriptor startVertex = currentEntity + realStart; //@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    //Lattice::VertexDescriptor headVertex = lattice::getVertex(
    //        lattice, currentEntity + realHead);
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, realHead, currentEntity);
    //Lattice::VertexDescriptor headVertex = currentEntity + realHead; //@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    //Lattice::VertexDescriptor endVertex = lattice::getVertex(
    //        lattice, currentEntity + realEnd);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, realEnd, currentEntity);
    //Lattice::VertexDescriptor endVertex = currentEntity + realEnd; //@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    //Lattice::EdgeDescriptor edgeStart2 = lattice.firstOutEdge(startVertex,
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = lattice::getTopEdges(
            lattice, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, endVertex);
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

bool GroupAction::test(Lattice &lattice, int,
        RuleTokenSizes &ruleTokenSizes) {
    if ( ( (size_t) lattice.getLastVertex() ) < head) {
        return false;
    }
    if (ruleTokenSizes[head - 1] == 0) {
        //@todo: throw wyjatek
//        std::cerr << "Element group: " << head - 1 << " empty!" << std::endl;
        return false;
    }
    return true;
}

std::string GroupAction::getGroup() const {
    return group;
}

unsigned int GroupAction::getHead() const {
    return head;
}

void GroupAction::setGroup(std::string aGroup) {
    group = aGroup;
}

void GroupAction::setHead(unsigned int aHead) {
    head = aHead;
}

unsigned int GroupAction::getStart() const {
    return start;
}

void GroupAction::setStart(unsigned int aStart) {
    start = aStart;
}

unsigned int GroupAction::getEnd() const {
    return end;
}

void GroupAction::setEnd(unsigned int aEnd) {
    end = aEnd;
}

std::string GroupAction::getRuleName() const {
    return ruleName;
}

void GroupAction::setRuleName(std::string aRuleName) {
    ruleName = aRuleName;
}

void GroupAction::init(std::string aGroup, unsigned int aStart,
        unsigned int aEnd, unsigned int aHead,
        std::string aRuleName) {
    group = aGroup;
    start = aStart;
    end = aEnd;
    head = aHead;
    if ((head > (end+1)) || (head <= start))
    {
        if (start == end)
            head = 0;
        else
            head = (head % (end+1)) - start;
    }
    type = "group";
    ruleName = aRuleName;
    verbose = false;
}


}

}

}


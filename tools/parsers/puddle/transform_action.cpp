
#include "transform_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

//#include "TransitionGraph.hpp"
//#include "ParseGraph.hpp"

namespace poleng
{
namespace bonsai
{
    namespace puddle
    {

TransformAction::TransformAction(std::string aGroup, int aElement, std::string aRuleName, LatticeWrapperPtr aLatticeWrapper)
{
    group = aGroup;
    element = aElement; // - 1;
//    std::cout << "Grupe zalozylem od " << start << " do " << end << " z glowa w: " << head << " dla reguly: " << aRuleName <<  std::endl;
    type = "transform";
    ruleName = aRuleName;
    verbose = false;
}

TransformAction::~TransformAction()
{
}

//bool TransformAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
bool TransformAction::apply(ParseGraphPtr pg, Lattice &lattice,
        int currentEntity, std::vector<int> matchedTokensSize) {

    int before = 0;
    int i = 0;
    while (i < (element - 1))
    {
        before += matchedTokensSize[i];
        i ++;
    }

//    Group *gr = (Group*)(entities[currentEntity + before]);

//    gr->setRuleName(ruleName);
//    gr->setGroupType(group);

    TransitionInfo *edge = util::getEdge(pg, currentEntity, before);
    edge->setLabel(group);
    Lattice::VertexDescriptor startVertex = currentEntity + before; //@todo: czy te numerki tu sie kupy trzymaja trzeba sprawdzic
    Lattice::VertexDescriptor headVertex = currentEntity + before;
    Lattice::VertexDescriptor endVertex = currentEntity + before;
    std::list<Lattice::EdgeDescriptor> startEdges = latticeWrapper->getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = latticeWrapper->getTopEdges(
            lattice, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = latticeWrapper->getTopEdges(
            lattice, endVertex);
    latticeWrapper->removeParseEdges(lattice, headVertex, headVertex + 1);
    std::list<Lattice::EdgeSequence> groupPartitions =
        latticeWrapper->getEdgesRange(
                lattice, startVertex, endVertex
                );
    latticeWrapper->addParseEdges(
            lattice,
            startEdges,
            endEdges,
            this->group,
            headEdges,
            groupPartitions
            );
/*    Edges::iterator e = edges.begin();
    int index = -1;
    i = 0;
    while (e != edges.end())
    {
        if ((*e)->getId() == gr->getId())
        {
            index = i;
            (*e)->setLabel(group);
            break;
        }
        i ++;
        e ++;
    }

        if (index < 0)
        {
            std::cerr << "Could not find edge to transform. Id: " << gr->getId() << "." << std::endl;
            return false;
        }*/

    return true;
}

//bool TransformAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
bool TransformAction::test(ParseGraphPtr pg, Lattice &lattice,
        int currentEntity, std::vector<int> matchedTokensSize) {
    //if (entities.size() < element)
    if ( (pg->num_vertices() - 1) < element ) {
        return false;
    }
    if (matchedTokensSize[element - 1] == 0)
    {
        std::cerr << "Element transform: " << element - 1 << " empty!" << std::endl;
        return false;
    }

    int before = 0;
    int i = 0;
    while (i < (element - 1))
    {
        before += matchedTokensSize[i];
        i ++;
    }

    //if (entities[currentEntity + before]->getType() != "group")
    TransitionInfo *edge = util::getEdge(pg, currentEntity, before);
    if (edge->getType() != "group") {
        return false;
    }
    return true;
}

std::string TransformAction::getGroup()
{
    return group;
}

int TransformAction::getElement()
{
    return element;
    //return (head + 1);
}

void TransformAction::setGroup(std::string aGroup)
{
    group = aGroup;
}

void TransformAction::setElement(int aElement)
{
    element = aElement; // - 1;
}

std::string TransformAction::getRuleName()
{
    return ruleName;
}

void TransformAction::setRuleName(std::string aRuleName)
{
    ruleName = aRuleName;
}

}

}

}



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

TransformAction::TransformAction(std::string aGroup, int aElement, std::string aRuleName) //, LatticeWrapperPtr aLatticeWrapper)
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
//bool TransformAction::apply(ParseGraphPtr pg, Lattice &lattice,
bool TransformAction::apply(Lattice &lattice, int currentEntity,
        std::vector<int> matchedTokensSize) {

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

//    TransitionInfo *edge = util::getEdge(pg, currentEntity, before);
//    edge->setLabel(group);
    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, currentEntity + before); //@todo: czy te numerki tu sie kupy trzymaja trzeba sprawdzic
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, currentEntity + before);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, currentEntity + before);
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
            element - 1
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
//bool TransformAction::test(ParseGraphPtr pg, Lattice &lattice,
bool TransformAction::test(Lattice &lattice, int currentEntity,
        std::vector<int> matchedTokensSize) {
    //if (entities.size() < element)
    //if ( (pg->num_vertices() - 1) < element ) {
    if ( (lattice.getLastVertex()) < element ) {
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
//    TransitionInfo *edge = util::getEdge(pg, currentEntity, before);
//    if (edge->getType() != "group") {
    //Lattice::VertexDescriptor vertex = currentEntity + before;
    Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
            currentEntity + before);
    //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
    while (lattice::getTopEdges(lattice, vertex).size() == 0) { //if there is no edge at a given position, proceed to the next vertex, as it may be a whitespace
        before ++;
        vertex = currentEntity + before;
    }

    std::list<Lattice::EdgeDescriptor> edges = lattice::getTopEdges(
            lattice, vertex);
    if (! edges.empty()) {
        LayerTagCollection tags = lattice.getEdgeLayerTags(edges.front());
        if (! lattice.getLayerTagManager().match(
                    lattice.getLayerTagManager().getMask(tags),
                    "parse"))
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


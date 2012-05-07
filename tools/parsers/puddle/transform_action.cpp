#include "transform_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace poleng {
namespace bonsai {
    namespace puddle
    {

TransformAction::TransformAction(std::string aGroup, int aElement,
        std::string aRuleName) {
    init(aGroup, aElement, aRuleName);
}

bool TransformAction::apply(Lattice &lattice, int matchedStartIndex,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence> &rulePartitions) {

    int before = util::getTransformActionParams(ruleTokenSizes, element);

    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, before, matchedStartIndex);
    //@todo: czy te numerki tu sie kupy trzymaja trzeba sprawdzic
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, before, matchedStartIndex);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, before, matchedStartIndex);
    lattice::removeParseEdges(lattice, headVertex, headVertex + 1);
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = lattice::getTopEdges(
            lattice, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, endVertex);
    if (startEdges.empty() || headEdges.empty() || endEdges.empty()) {
        return false;
    }
    rulePartitions =  lattice::getEdgesRange(lattice, startVertex, endVertex);
    lattice::addParseEdges(
            lattice,
            startEdges,
            endEdges,
            this->group,
            headEdges,
            rulePartitions,
            element - 1
            );
    return true;
}

bool TransformAction::test(Lattice &lattice, int matchedStartIndex,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    if ( (lattice.getLastVertex()) < element ) {
        return false;
    }
    if (ruleTokenSizes[element - 1] == 0) {
        throw PuddleRuleElementNotMatchedException(
                "Rule element " + boost::lexical_cast<std::string>(element)
                + " is empty"
                );
    }

    int before = util::getTransformActionParams(ruleTokenSizes, element);

    //Lattice::VertexDescriptor vertex = matchedStartIndex + before;
    Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
            before, matchedStartIndex);
//    //@todo: czy to sprawdzenie jest nadal konieczne?
//    ta funkcja getVertex nie robi czegos takiego?
//    while (lattice::getTopEdges(lattice, vertex).size() == 0) {
//    //if there is no edge at a given position, proceed to the next vertex,
//    as it may be a whitespace
//        before ++;
//        vertex = matchedStartIndex + before;
//    }

    std::list<Lattice::EdgeDescriptor> edges = lattice::getTopEdges(
            lattice, vertex);
    if (! edges.empty()) {
        LayerTagCollection tags = lattice.getEdgeLayerTags(edges.front());
        if (! lattice.getLayerTagManager().isThere("parse", tags))
            return false;
    }
    return true;
}

void TransformAction::init(std::string aGroup, int aElement,
        std::string aRuleName) {
    group = aGroup;
    element = aElement;
    type = "transform";
    ruleName = aRuleName;
}

}

}

}

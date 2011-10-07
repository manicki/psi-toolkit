
#include "syntok_action.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

SyntokAction::SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndices,
        std::vector<Morphology> aMorphology, std::string aRuleName,
        std::string uMorphology) {
    init(aStart, aEnd, aTokenIndices, aMorphology, aRuleName, uMorphology);
}

SyntokAction::~SyntokAction() { }

//bool SyntokAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
//@todo: opisac dokladnie, co konkretnie po kolei jest robione w ramach implementacji tej akcji (i z kazda akcja co jest robione). pobranie czego, wyliczenie czego, zebranie jakich form, dodanie jakich krawedzi, zastapienie/usuniecie czego, skad
//1. ustalenie skad zaczynamy
//2. zebranie scalonych form bazowych
//3. wstawienie nowej krawedzi typu token albo grup (w zaleznosci od przelacznika) NOWOSC: wczesniej bylo, ze albo dawaj krawedz group albo nic. teraz musi byc krawedz ,jak graf to jedyna struktura. ewentualnei mozna dac na koniec opcje, wywal syntoki z grafu, przeliczyc reszte grafu
bool SyntokAction::apply(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {
    int realStart;
    int realEnd;
    util::getSyntokActionParams(ruleTokenSizes, start, end, realStart, realEnd);

    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, realStart, currentEntity);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, realEnd, currentEntity);
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, endVertex);

    std::list<Lattice::EdgeSequence> edgeSequences =
        lattice::getEdgesRange(
                lattice, startVertex, endVertex
                );
    std::vector<std::string> baseForms = generateBaseForms(lattice,
            edgeSequences);
    std::string concatenatedOrth = generateOrth(lattice, edgeSequences);
    std::string syntokCategory = concatenatedOrth;
    LayerTagCollection tags = lattice.getLayerTagManager().createSingletonTagCollection("form");
    if (syntok) {
        syntokCategory = "SYNTOK";
        tags =
            lattice.getLayerTagManager().createSingletonTagCollection("parse");
    }
    lattice::addSyntokEdges(
            lattice,
            startEdges,
            endEdges,
            syntokCategory,
            concatenatedOrth,
            baseForms,
            morphology,
            edgeSequences,
            tags);
    return true;
}

bool SyntokAction::test(Lattice &, int,
        RuleTokenSizes &ruleTokenSizes) {
    for (std::vector<int>::iterator i = ruleTokenSizes.begin();
            i != ruleTokenSizes.end(); ++ i) {
        if (*i > 0)
            return true;
    }
    return false;
}

int SyntokAction::getStart() const {
    return start;
}

void SyntokAction::setStart(int aStart) {
    start = aStart;
}

int SyntokAction::getEnd() const {
    return end;
}

void SyntokAction::setEnd(int aEnd) {
    end = aEnd;
}

std::vector<Morphology> SyntokAction::getMorphology() const {
    return morphology;
}

void SyntokAction::setMorphology(std::vector<Morphology> aMorphology) {
    morphology = aMorphology;
}

std::vector<int> SyntokAction::getTokenIndices() const {
    return tokenIndices;
}

void SyntokAction::setTokenIndices(std::vector<int> aTokenIndices) {
    tokenIndices = aTokenIndices;
}

std::string SyntokAction::getRuleName() const {
    return ruleName;
}

void SyntokAction::setRuleName(std::string aRuleName) {
    ruleName = aRuleName;
}

std::string SyntokAction::getUMorphology() const {
    return morphology_;
}

void SyntokAction::init(int aStart, int aEnd, std::vector<int> aTokenIndices,
        std::vector<Morphology> aMorphology, std::string aRuleName,
        std::string uMorphology) {
    type = "syntok";
    start = aStart;
    end = aEnd;
    ruleName = aRuleName;
    verbose = false;
    syntok = false;
    tokenIndices = aTokenIndices;
    morphology = aMorphology;
    morphology_ = uMorphology;
}

    //gets the base forms of all matched tokens. permutates those forms
    //in order to create a list of concatenated base forms.
    //those forms become the base forms of the added syntok
std::vector<std::string> SyntokAction::generateBaseForms(Lattice &lattice,
        std::list<Lattice::EdgeSequence> edgeSequences) {
    std::vector<std::string> baseForms;
    for (std::list<Lattice::EdgeSequence>::iterator sequenceIt =
            edgeSequences.begin();
            sequenceIt != edgeSequences.end();
            ++ sequenceIt) {
        std::string baseForm = "";
        int index = lattice.getEdgeBeginIndex(sequenceIt->firstEdge());
        for (Lattice::EdgeSequence::Iterator edgeIt = sequenceIt->begin();
                edgeIt != sequenceIt->end();
                ++ edgeIt) {
            AnnotationItem ai = lattice.getEdgeAnnotationItem(*edgeIt);
            if (lattice::isDiscarded(lattice, *edgeIt))
                continue;

            std::string base = lattice::getBase(lattice, *edgeIt);

            if (index != lattice.getEdgeBeginIndex(*edgeIt)) {
                baseForm += " ";
                index ++;
            }
            baseForm += base;
            index += lattice.getEdgeLength(*edgeIt);
        }
        baseForms.push_back(baseForm);
    }
    return baseForms;
}

std::string SyntokAction::generateOrth(Lattice &lattice,
        std::list<Lattice::EdgeSequence> edgeSequences) {
    std::string concatenatedOrth = "";
    int index = lattice.getEdgeBeginIndex(edgeSequences.front().firstEdge());
    for (Lattice::EdgeSequence::Iterator edgeIt = edgeSequences.front().begin();
            edgeIt != edgeSequences.front().end();
            ++ edgeIt) {
        if (index != lattice.getEdgeBeginIndex(*edgeIt)) {
            concatenatedOrth += " ";
            index ++;
        }
        concatenatedOrth += lattice.getEdgeText(*edgeIt);
        index += lattice.getEdgeLength(*edgeIt);
    }
    return concatenatedOrth;
}


}

}

}

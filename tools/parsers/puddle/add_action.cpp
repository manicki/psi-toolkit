
#include "add_action.hpp"
#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

AddAction::AddAction(std::vector<Morphology> aInterpretations, std::string aBase,
        int aTokenIndex) {
    init (aInterpretations, aBase, aTokenIndex);
}

bool AddAction::apply(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {
    int count = ruleTokenSizes[tokenIndex - 1];
    if (count == 0) {
//        std::cout << "Nothing matched to " << tokenIndex << " in ...." << std::endl;
        return true;
    }
    int before = util::getAddActionParams(ruleTokenSizes, tokenIndex);

    Lattice::VertexDescriptor startVertex = lattice::getVertex(lattice,
            before, currentEntity);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(lattice,
            before + count - 1, currentEntity);

    addEdges(lattice, startVertex, endVertex);

    return true;
}

bool AddAction::test(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {

    int count = ruleTokenSizes[tokenIndex - 1];
    if (count == 0) {
//        if (verbose)
//            std::cerr << "Nothing matched to " << tokenIndex << " in add ...." << std::endl;
        return true;
    }
    int before = util::getAddActionParams(ruleTokenSizes, tokenIndex);

    bool ret = true; //@todo: moze by tak zmienic ret w akcjach na cos w stylu to_apply

    Lattice::VertexDescriptor startVertex = lattice::getVertex(lattice,
            before, currentEntity);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(lattice,
            before + count - 1, currentEntity);
    ret = checkInterpretationsToAdd(lattice, startVertex, endVertex);

    return ret;
}

void AddAction::init(std::vector<Morphology> aInterpretations, std::string aBase,
        int aTokenIndex) {
    base = aBase;
    tokenIndex = aTokenIndex;
    type = "add";
    interpretations = aInterpretations;
}

    //check whether the interpretation(s) (with the given or all base forms
    //of the token) which is/are to be added is/are not already in token's
    //morphological features
bool AddAction::checkInterpretationsToAdd(Lattice &lattice,
        Lattice::VertexDescriptor startVertex,
        Lattice::VertexDescriptor endVertex) {
    bool ret = true;
    bool allBaseForms = (base == "[^<>]+");
    std::list<Lattice::EdgeSequence> edgeSequences = lattice::getEdgesRange(
            lattice, startVertex, endVertex);
    for (std::list<Lattice::EdgeSequence>::iterator sequenceIt = edgeSequences.begin();
            sequenceIt != edgeSequences.end(); ++ sequenceIt) {
        Lattice::EdgeSequence::Iterator edgeIt(lattice, *sequenceIt);
        while (edgeIt.hasNext()) {
            Lattice::EdgeDescriptor ed = edgeIt.next();
            AnnotationItem ai = lattice.getEdgeAnnotationItem(ed);
            if (lattice::isDiscarded(lattice, ed))
                continue; //skip discarded interpretations
            std::string edgeBase = lattice::getBase(lattice, ed);
            if (! allBaseForms) {
                if (edgeBase != base)
                    continue; //take the next variant
            }
            bool interpretationFound = false;
            for (std::vector<Morphology>::iterator morph_it =
                    interpretations.begin();
                    morph_it != interpretations.end();
                    ++ morph_it) {
                if (edgeBase == morph_it->begin()->second) { //@todo: poprawic to na porownanie z napisem zrobionym z mapy. swoja droga, ten warunek to ma sens?
                    interpretationFound = true;
                    break;
                }
            }
            if (interpretationFound) { //if the interpration in the token found, finish testing and do not add the interpration again
                ret = false;
                break;
            }
        }
    }
    return ret;
}

void AddAction::addEdges(Lattice &lattice, Lattice::VertexDescriptor startVertex,
        Lattice::VertexDescriptor endVertex) {
    bool allBaseForms = (base == "[^<>]+");
    std::list<Lattice::EdgeSequence> edgeSequences = lattice::getEdgesRange(
            lattice, startVertex, endVertex);
    for (std::list<Lattice::EdgeSequence>::iterator sequenceIt = edgeSequences.begin();
            sequenceIt != edgeSequences.end(); ++ sequenceIt) {
        Lattice::EdgeSequence::Iterator edgeIt(lattice, *sequenceIt);
        while (edgeIt.hasNext()) {
            Lattice::EdgeDescriptor ed = edgeIt.next();
            AnnotationItem ai = lattice.getEdgeAnnotationItem(ed);
            if (lattice::isDiscarded(lattice, ed))
                continue; //skip discarded interpretations
            if (! allBaseForms) {
                std::vector<std::string> baseForms;
                baseForms.push_back(base);
                lattice::addNewVariantEdges(lattice, ed,
                        baseForms, interpretations);
            } else {
                std::vector<std::string> baseForms;
                std::string baseForm = lattice::getBase(lattice, ed);
                baseForms.push_back(baseForm);
                lattice::addNewVariantEdges(lattice, ed,
                        baseForms, interpretations);
            }
        }
            if (! allBaseForms) {
                break;
                //when base form is explicitly given, take the orth form of the edge an go to the next vertex
            }
    }
}


}

}

}

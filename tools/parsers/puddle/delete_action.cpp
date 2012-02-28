#include "delete_action.hpp"
#include <iostream>

namespace poleng {

    namespace bonsai {
        namespace puddle {

            DeleteAction::DeleteAction(DeleteConditions aConditions, int aTokenIndex) {
                init(aConditions, aTokenIndex);
            }

bool DeleteAction::apply(Lattice &lattice, int matchedStartIndex,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    int count = ruleTokenSizes[tokenIndex - 1];
    if (count == 0) {
//        std::cout << "Nothing matched to " << tokenIndex << " in ...." << std::endl;
        return true;
    }

    int before = util::getDeleteActionParams(ruleTokenSizes, tokenIndex);
    //Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
    //        matchedStartIndex + before);
    //Lattice::VertexDescriptor vertex = matchedStartIndex; //@todo: tymczasowo.
    //to nie uwzglednia lewego kontekstu
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

    lattice::deleteEdges(lattice, vertex, count, conditions);

    return true;
}

bool DeleteAction::test(Lattice &lattice,
        int matchedStartIndex, RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    bool ret = false;

    int count = ruleTokenSizes[tokenIndex - 1];
    if (count == 0) {
//        if (verbose)
//            std::cerr << "Nothing matched to " << tokenIndex << " in delete ...." << std::endl;
        return true;
    }
    int before = 0;
    int i = 0;
    while (i < (tokenIndex - 1)) {
        before += ruleTokenSizes[i];
        i ++;
    }
    //Lattice::VertexDescriptor vertex = matchedStartIndex + before;
    //Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
    //        matchedStartIndex + before);
    //Lattice::VertexDescriptor vertex = matchedStartIndex; //@todo: tymczasowo.
    //to nie uwzglednia lewego kontekstu
    Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
            before, matchedStartIndex);
//    //@todo: czy to sprawdzenie jest nadal konieczne?
//    //ta funkcja getVertex nie robi czegos takiego?
//    while (lattice::getTopEdges(lattice, vertex).size() == 0) {
//    //if there is no edge at a given position, proceed to the next vertex,
//    as it may be a whitespace
//        before ++;
//        vertex = matchedStartIndex + before;
//    }
    bool foundToDelete = foundEdgesToDelete(lattice, vertex, count);

    //@todo: ten warunek to tak ma byc? w ogole ten test musi byc taki zlozony?
    if (foundToDelete) {
        ret = true;
    } else {
        ret = true;
    }

    return ret;
}

void DeleteAction::init(DeleteConditions aConditions, int aTokenIndex) {
    tokenIndex = aTokenIndex;
    conditions = aConditions;
    type = "delete";
}

bool DeleteAction::foundEdgesToDelete(Lattice &lattice,
        Lattice::VertexDescriptor vertex, int count) {
    bool foundToDelete = false;
    int offset = vertex;
    int vertexI = 0;
    while (vertexI < count) {
        vertex = lattice::getVertex(lattice, vertexI, offset);
        std::list<Lattice::EdgeDescriptor> edges =
            lattice::getTopEdges(lattice, vertex);
        for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt = edges.begin();
                edgeIt != edges.end(); ++ edgeIt) {
            bool conditionsSatisfied = true;
            AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(*edgeIt);
            if (lattice::isDiscarded(lattice, *edgeIt))
                continue; //skip discarded edges

            for (DeleteConditions::iterator cond_it = conditions.begin();
                    cond_it != conditions.end(); ++ cond_it) {
                bool satisfied = true;
                if (cond_it->type == BASE_CONDITION) {
                    std::string tokenBase = lattice.getAnnotationItemManager().
                        getValue(annotationItem, "base");
                    if (cond_it->negation) {
                        if (RegExp::FullMatch(tokenBase, cond_it->pattern)) {
                            satisfied = false;
                            break;
                        }
                    } else {
                        if (!RegExp::FullMatch(tokenBase, cond_it->pattern)) {
                            satisfied = false;
                            break;
                        }
                    }
                } else if (cond_it->type == MORPHOLOGY_CONDITION) {
                    std::string tokenMorphology = lattice::getMorphologyString(
                            lattice, *edgeIt);
                    if (!RegExp::FullMatch(tokenMorphology, cond_it->pattern)) {
                        satisfied = false;
                        break;
                    }
                }
                if (! satisfied) {
                    conditionsSatisfied = false;
                    break;
                }
            }
            if ( conditionsSatisfied) {
                foundToDelete = true;
                //@todo: i tu break?
            }
        }
        vertexI ++;
    }
    return foundToDelete;
}


}

}

}

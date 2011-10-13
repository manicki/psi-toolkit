
#include "delete_action.hpp"
#include <iostream>

namespace poleng {

    namespace bonsai {
        namespace puddle {

            DeleteAction::DeleteAction(DeleteConditions aConditions, int aTokenIndex,
                    std::string uPattern) {
                init(aConditions, aTokenIndex, uPattern);
            }

bool DeleteAction::apply(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {
    int count = ruleTokenSizes[tokenIndex - 1];
    if (count == 0) {
//        std::cout << "Nothing matched to " << tokenIndex << " in ...." << std::endl;
        return true;
    }

    int before = util::getDeleteActionParams(ruleTokenSizes, tokenIndex);
    //Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
    //        currentEntity + before);
    //Lattice::VertexDescriptor vertex = currentEntity; //@todo: tymczasowo. to nie uwzglednia lewego kontekstu
    Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
            before, currentEntity);
    //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
    while (lattice::getTopEdges(lattice, vertex).size() == 0) { //if there is no edge at a given position, proceed to the next vertex, as it may be a whitespace
        before ++;
        vertex = currentEntity + before;
    }

    lattice::deleteEdges(lattice, vertex, count, conditions);

    return true;
}

bool DeleteAction::test(Lattice &lattice,
        int currentEntity, RuleTokenSizes &ruleTokenSizes) {
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
    //Lattice::VertexDescriptor vertex = currentEntity + before;
    //Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
    //        currentEntity + before);
    //Lattice::VertexDescriptor vertex = currentEntity; //@todo: tymczasowo. to nie uwzglednia lewego kontekstu
    Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
            before, currentEntity);
    //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
    while (lattice::getTopEdges(lattice, vertex).size() == 0) { //if there is no edge at a given position, proceed to the next vertex, as it may be a whitespace
        before ++;
        vertex = currentEntity + before;
    }
    bool foundToDelete = foundEdgesToDelete(lattice, vertex, count);

    //@todo: ten warunek to tak ma byc? w ogole ten test musi byc taki zlozony?
    if (foundToDelete) {
        ret = true;
    } else {
        ret = true;
    }

    return ret;
}

DeleteConditions DeleteAction::getConditions() const {
    return conditions;
}

int DeleteAction::getTokenIndex() const {
    return tokenIndex;
}

void DeleteAction::setTokenIndex(int aTokenIndex) {
    tokenIndex = aTokenIndex;
}

std::string DeleteAction::getUPattern() const {
    return pattern_;
}

void DeleteAction::init(DeleteConditions aConditions, int aTokenIndex,
                    std::string uPattern) {
    tokenIndex = aTokenIndex;
    pattern_ = uPattern;
    conditions = aConditions;
    verbose = false;
    type = "delete";
}

bool DeleteAction::foundEdgesToDelete(Lattice &lattice,
        Lattice::VertexDescriptor vertex, int count) {
    bool foundToDelete = false;
    int offset = vertex;
    int vertexI = 0;
    while (vertexI < count) {
        vertex = lattice::getVertex(lattice, offset + vertexI);
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

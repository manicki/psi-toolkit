
#include "unify_action.hpp"
#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

UnifyAction::UnifyAction(std::vector<std::string> aUnifiedPatterns,
        std::vector<std::string> aUnifiedAttributes,
        std::vector<int> aTokenIndices) {
    init(aUnifiedPatterns, aUnifiedAttributes, aTokenIndices);
}


UnifyAction::~UnifyAction() { }

bool UnifyAction::apply(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {

    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
            pattern_it != unifiedPatterns.end(); ++ pattern_it) {

        std::set<std::string> refValues;
        for (std::vector<int>::iterator index_it = tokenIndices.begin();
                index_it != tokenIndices.end(); ++ index_it) {

            int count = ruleTokenSizes[*index_it - 1];
            if (count == 0) {
                continue;
            }
            int before = 0;
            int i = 0;
            while (i < (*index_it - 1)) {
                before += ruleTokenSizes[i];
                i ++;
            }

            //Lattice::VertexDescriptor vertex = currentEntity + before;
            Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                    before, currentEntity);
            //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
            while (lattice::getTopEdges(lattice, vertex).size() == 0) {
                before ++;
                vertex = currentEntity + before;
            }

            int offset = vertex;
            int vertexI = 0;
            //for (vertex = currentEntity + before;
            //        vertex < (currentEntity + before + count); vertex ++) {
            while (vertexI < count) {
                vertex = lattice::getVertex(lattice, offset + vertexI);
                std::list<Lattice::EdgeDescriptor> edges =
                    lattice::getTopEdges(lattice, vertex);
                std::set<std::string> values;
                //                TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
                //                for (std::vector<PosInfo>::iterator var_it =
                //                        edge->variants_.begin();
                //                        var_it != edge->variants_.end();
                //                        var_it ++) {
                //                    if (! boost::get<2>(*var_it) )
                //                        continue;
                //                    std::string morphology = boost::get<1>(*var_it);
                for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                        edges.begin();
                        edgeIt != edges.end(); ++ edgeIt) {
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(*edgeIt);
                    //if (lattice.getAnnotationItemManager().getValue(
                    //            annotationItem, "discard") == "1")
                    if (lattice::isDiscarded(lattice, *edgeIt))
                        continue; //skip discarded edges
                    //std::string morphology = lattice.getAnnotationItemManager().
                    //    getValue(annotationItem, "morphology");
                    //std::string morphology =
                    //    lattice::getPartOfSpeech(lattice, *edgeIt);
                    //std::string morpho = lattice.getAnnotationItemManager().
                    //    getValue(annotationItem, "morpho");
                    //if (morpho != "")
                    //    morphology += ":" + morpho;
                    std::string morphology = lattice::getMorphologyString(
                            lattice, *edgeIt);

                    std::string slot;
                    std::string value;
                    if (RegExp::FullMatch(morphology, *pattern_it, &slot, &value)) {
                        values.insert(value);
                    }

                    if (! values.empty()) {
                        if (refValues.empty()) {
                            refValues.insert(values.begin(), values.end());
                        } else {
                            std::set<std::string> intersection;
                            std::set<std::string>::iterator it;
                            std::set_intersection(refValues.begin(), refValues.end(),
                                    values.begin(), values.end(),
                                    std::inserter(intersection, intersection.begin())
                                    );
                            refValues.clear();
                            refValues.insert(intersection.begin(), intersection.end());
                        }
                    }
                }
                vertexI ++;
            }
            }

            for (std::vector<int>::iterator index_it = tokenIndices.begin();
                    index_it != tokenIndices.end(); ++ index_it) {

                int count = ruleTokenSizes[*index_it - 1];
                if (count == 0) {
                    continue;
                }
                int before = 0;
                int i = 0;
                while (i < (*index_it - 1)) {
                    before += ruleTokenSizes[i];
                    i ++;
                }
                //Lattice::VertexDescriptor vertex = currentEntity + before;
                Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                        before, currentEntity);
                //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
                while (lattice::getTopEdges(lattice, vertex).empty()) {
                    before ++;
                    vertex = currentEntity + before;
                }

                int offset = vertex;
                int vertexI = 0;
                //for (vertex = currentEntity + before;
                //        vertex < (currentEntity + before + count); vertex ++) {
                while (vertexI < count) {
                    vertex = lattice::getVertex(lattice, offset + vertexI);
                    std::list<Lattice::EdgeDescriptor> edges =
                        lattice::getTopEdges(lattice, vertex);
                    for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                            edges.begin();
                            edgeIt != edges.end(); ++ edgeIt) {
                        AnnotationItem annotationItem =
                            lattice.getEdgeAnnotationItem(*edgeIt);
                        //if (lattice.getAnnotationItemManager().getValue(
                        //            annotationItem, "discard") == "1")
                        if (lattice::isDiscarded(lattice, *edgeIt))
                            continue; //skip discarded edges
                        //std::string morphology = lattice.getAnnotationItemManager().
                        //    getValue(annotationItem, "morphology");
                        //std::string morphology =
                        //    lattice::getPartOfSpeech(lattice, *edgeIt);
                        //std::string morpho = lattice.getAnnotationItemManager().
                        //    getValue(annotationItem, "morphology");
                        //if (morpho != "")
                        //    morphology += ":" + morpho;
                        std::string morphology = lattice::getMorphologyString(
                                lattice, *edgeIt);

                        std::string slot;
                        std::string value;
                        //if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
                        if (RegExp::FullMatch(morphology, *pattern_it, &slot, &value)) {
                            std::set<std::string>::iterator unifIt = refValues.find(value);
                            if (unifIt == refValues.end()) {
                                //boost::get<2>(*var_it) = 0; //delete the variant
                                //discard the edge
                                lattice.getAnnotationItemManager().setValue(annotationItem,
                                        "discard", "1");
                            }
                        }
                    }
                    vertexI ++;
                }

                //attribute_it ++;
            }
        }
    return true;
//    std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
//    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
//            pattern_it != unifiedPatterns.end(); pattern_it ++) {
//
//        for (std::vector<int>::iterator index_it = tokenIndices.begin();
//                index_it != tokenIndices.end(); index_it ++) {
//
//            int count = matchedTokensSize[*index_it - 1];
//            if (count == 0) {
//                continue;
//            }
//            int before = 0;
//            int i = 0;
//            while (i < (*index_it - 1)) {
//                before += matchedTokensSize[i];
//                i ++;
//            }
//
//            for (int edge_i = before; edge_i < (before + count); edge_i ++) {
//                TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
//                for (std::vector<PosInfo>::iterator var_it =
//                        edge->variants_.begin();
//                        var_it != edge->variants_.end();
//                        var_it ++) {
//                    if (! boost::get<2>(*var_it) )
//                        continue;
//                    std::string morphology = boost::get<1>(*var_it);
//
//                    std::string slot;
//                    std::string value;
//                    if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
//                        bool valueFound = false;
//                        std::vector<std::string> unified = unifiedValues.find(*attribute_it)->second;
//                        for (std::vector<std::string>::iterator unifIt = unified.begin();
//                                unifIt != unified.end(); unifIt ++) {
//                            if (*unifIt == value) {
//                                valueFound = true;
//                                break;
//                            }
//                        }
//                        if (!valueFound)
//                            boost::get<2>(*var_it) = 0; //delete the variant
//                    }
//                }
//            }
//
//        }
//        attribute_it ++;
//    }
//    return true;
}

bool UnifyAction::test(Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {

    //unifiedValues.clear();
    bool toApply = true;
    std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
            pattern_it != unifiedPatterns.end(); ++ pattern_it) {

        bool wasAllNulls = false;
//        bool wasNotNull = false;

        std::set<std::string> refValues;
        for (std::vector<int>::iterator index_it = tokenIndices.begin();
                index_it != tokenIndices.end(); ++ index_it) {

            int count = ruleTokenSizes[*index_it - 1];
            if (count == 0) {
                continue;
            }
            int before = 0;
            int i = 0;
            while (i < (*index_it - 1)) {
                before += ruleTokenSizes[i];
                i ++;
            }

            //Lattice::VertexDescriptor vertex = currentEntity + before;
            Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                    before, currentEntity);
            //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
            while (lattice::getTopEdges(lattice, vertex).size() == 0) {
                before ++;
                vertex = currentEntity + before;
            }

            int offset = vertex;
            int vertexI = 0;
            //for (vertex = currentEntity + before;
            //        vertex < (currentEntity + before + count); vertex ++) {
            while (vertexI < count) {
                vertex = lattice::getVertex(lattice, offset + vertexI);
                //TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
                std::list<Lattice::EdgeDescriptor> edges =
                    lattice::getTopEdges(lattice, vertex);
                bool allValuesNull = true;
                std::set<std::string> values;
                for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                        edges.begin();
                        edgeIt != edges.end(); ++ edgeIt) {
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(*edgeIt);
                    //if (lattice.getAnnotationItemManager().getValue(
                    //            annotationItem, "discard") == "1")
                    if (lattice::isDiscarded(lattice, *edgeIt))
                        continue; //skip discarded edges
                //for (std::vector<PosInfo>::iterator var_it =
                //        edge->variants_.begin();
                //        var_it != edge->variants_.end();
                //        var_it ++) {
                //    if (! boost::get<2>(*var_it) )
                //        continue;
                    //std::string morphology = boost::get<1>(*var_it);
                    //std::string morphology = lattice.getAnnotationItemManager().
                    //    getValue(annotationItem, "morphology");
                    //std::string morphology =
                    //    lattice::getPartOfSpeech(lattice, *edgeIt);
                    //std::string morpho = lattice.getAnnotationItemManager().
                    //    getValue(annotationItem, "morpho");
                    //if (morpho != "")
                    //    morphology += ":" + morpho;
                    std::string morphology = lattice::getMorphologyString(
                            lattice, *edgeIt);

                    std::string slot;
                    std::string value;
                    //if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
                    if (RegExp::FullMatch(morphology, *pattern_it, &slot, &value)) {
                        allValuesNull = false;
                        values.insert(value);
                    }
                }
                if (! values.empty()) {
//                    wasNotNull = true;
                    if (refValues.empty()) {
                        refValues.insert(values.begin(), values.end());
                    } else {
                        std::set<std::string> intersection;
                        std::set<std::string>::iterator it;
                        std::set_intersection(refValues.begin(), refValues.end(),
                                values.begin(), values.end(),
                                std::inserter(intersection, intersection.begin())
                                );
                        refValues.clear();
                        refValues.insert(intersection.begin(), intersection.end());
                    }
                }
                if (allValuesNull && (! wasAllNulls)) {
                    wasAllNulls = true;
                }
                vertexI ++;
            }

        }
        if (! refValues.empty()) { //there are some common values of the attribute
            if (wasAllNulls) { //there is a token with no value of the attribute defined
                if (! nullAgreement)
                    toApply = false; //cannot apply
            }
        } else { //no common values of the attribute, cannot unify
            toApply = false;
        }
        if (! toApply)
            break;
//        else {
//            std::vector<std::string> attributeUnifiedValues(
//                    refValues.begin(), refValues.end());
//            unifiedValues.insert(std::pair<std::string, std::vector<std::string> >(
//                        *attribute_it, attributeUnifiedValues
//                        ));
//        }
        ++ attribute_it;
    }
    return toApply;
//    bool toApply = true;
//    std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
//    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
//            pattern_it != unifiedPatterns.end(); pattern_it ++) {
//
//        bool wasAllNulls = false;
//        bool wasNotNull = false;
//
//        std::set<std::string> refValues;
//        for (std::vector<int>::iterator index_it = tokenIndices.begin();
//                index_it != tokenIndices.end(); index_it ++) {
//
//            int count = matchedTokensSize[*index_it - 1];
//            if (count == 0) {
//                continue;
//            }
//            int before = 0;
//            int i = 0;
//            while (i < (*index_it - 1)) {
//                before += matchedTokensSize[i];
//                i ++;
//            }
//
//            for (int edge_i = before; edge_i < (before + count); edge_i ++) {
//                TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
//                bool allValuesNull = true;
//                std::set<std::string> values;
//                for (std::vector<PosInfo>::iterator var_it =
//                        edge->variants_.begin();
//                        var_it != edge->variants_.end();
//                        var_it ++) {
//                    if (! boost::get<2>(*var_it) )
//                        continue;
//                    std::string morphology = boost::get<1>(*var_it);
//
//                    std::string slot;
//                    std::string value;
//                    if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
//                        allValuesNull = false;
//                        values.insert(value);
//                    }
//                }
//                if (values.size() > 0) {
//                    wasNotNull = true;
//                    if (refValues.size() == 0) {
//                        refValues.insert(values.begin(), values.end());
//                    } else {
//                        std::set<std::string> intersection;
//                        std::set<std::string>::iterator it;
//                        std::set_intersection(refValues.begin(), refValues.end(),
//                                values.begin(), values.end(),
//                                std::inserter(intersection, intersection.begin())
//                                );
//                        refValues.clear();
//                        refValues.insert(intersection.begin(), intersection.end());
//                    }
//                }
//                if (allValuesNull && (! wasAllNulls)) {
//                    wasAllNulls = true;
//                }
//            }
//
//        }
//        if (refValues.size() > 0) { //there are some common values of the attribute
//            if (wasAllNulls) { //there is a token with no value of the attribute defined
//                if (! nullAgreement)
//                    toApply = false; //cannot apply
//            }
//        } else { //no common values of the attribute, cannot unify
//            toApply = false;
//        }
//        if (! toApply)
//            break;
//        else {
//            std::vector<std::string> attributeUnifiedValues(
//                    refValues.begin(), refValues.end());
//            unifiedValues.insert(std::pair<std::string, std::vector<std::string> >(
//                        *attribute_it, attributeUnifiedValues
//                        ));
//        }
//        attribute_it ++;
//    }
//    return toApply;
}


std::vector<std::string> UnifyAction::getPatterns() const {
    return unifiedPatterns;
}

void UnifyAction::setPatterns(std::vector<std::string> aPatterns) {
    unifiedPatterns = aPatterns;
}

std::vector<std::string> UnifyAction::getAttributes() const {
    return unifiedAttributes;
}

void UnifyAction::setAttributes(std::vector<std::string> aAttributes) {
    unifiedAttributes = aAttributes;
}

std::vector<int> UnifyAction::getTokenIndices() const {
    return tokenIndices;
}

void UnifyAction::setTokenIndices(std::vector<int> aTokenIndices) {
    tokenIndices =  aTokenIndices;
}

void UnifyAction::init(std::vector<std::string> aUnifiedPatterns,
        std::vector<std::string> aUnifiedAttributes,
        std::vector<int> aTokenIndices) {
    nullAgreement = true; //@todo: to ma byc parametrem parsera

    type = "unify";
    verbose = false;

    tokenIndices = aTokenIndices;
    unifiedAttributes = aUnifiedAttributes;
    unifiedPatterns = aUnifiedPatterns;
}


}

}

}

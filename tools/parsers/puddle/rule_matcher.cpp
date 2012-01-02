#include "rule_matcher.hpp"

#include <iostream>

namespace poleng {

namespace bonsai {
    namespace puddle {


RuleMatcher::RuleMatcher() {
    initProperties();
}

void RuleMatcher::setRules(RulesPtr aRules) {
    rules = aRules;
}

void RuleMatcher::applyRules(Lattice &lattice) {
    for (Rules::iterator ruleIt = rules->begin(); ruleIt != rules->end();
            ++ ruleIt) {
        std::string sentenceString = generateSentenceString(lattice);
        int matchedStartIndex;
        int afterIndex = 0;
        std::vector<StringPiece> match;
        std::string tmpSentenceString = sentenceString;
        while ( (matchedStartIndex = (*ruleIt)->matchPattern(tmpSentenceString,
                        afterIndex, match) ) > -1 ) {
            bool structureChanged = false;
            RuleTokenSizes ruleTokenSizes;
            std::list<Lattice::EdgeSequence> rulePartitions;
            std::string oldSentenceString = sentenceString;
            if ( (*ruleIt)->test(sentenceString, lattice, matchedStartIndex,
                        match, ruleTokenSizes, rulePartitions) ) {
                if ((*ruleIt)->apply(sentenceString, lattice, matchedStartIndex,
                            ruleTokenSizes, rulePartitions) ) {
                    sentenceString = generateSentenceString(lattice,
                            matchedStartIndex);
                    structureChanged = true;
                }
            }
            tmpSentenceString = generateSentenceString(lattice,
                    afterIndex);
            if ((*ruleIt)->getRepeat()) {
                if (oldSentenceString != sentenceString)
                    tmpSentenceString = sentenceString;
            } else {
                if (oldSentenceString == sentenceString)
                    sentenceString = tmpSentenceString;
            }
            if (structureChanged) {
                match.clear();
                continue;
            }
            match.clear();
        }
    }

}

void RuleMatcher::addPosEdges(Lattice &lattice) {
    LayerTagMask mask = lattice.getLayerTagManager().getMask(
            lattice.getLayerTagManager().
            createSingletonTagCollection("form")
            );

    std::multimap<Lattice::VertexDescriptor, std::string> posEdgesMap;
    Lattice::EdgesSortedBySourceIterator edgeIterator =
        lattice.edgesSortedBySource(mask);
    while (edgeIterator.hasNext()) {
        Lattice::EdgeDescriptor edge = edgeIterator.next();
        int start = lattice.getEdgeBeginIndex(edge);
        int end = start + lattice.getEdgeLength(edge);
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);
        if (lattice::isDiscarded(lattice, edge))
            continue; //skip discarded forms
        std::string partOfSpeech = lattice::getPartOfSpeech(lattice, edge);
        std::pair<
            std::multimap<Lattice::VertexDescriptor, std::string>::iterator,
            std::multimap<Lattice::VertexDescriptor, std::string>::iterator
                > posEdgesMapIt =
                posEdgesMap.equal_range(start);
        bool alreadyAdded = false;
        if (posEdgesMapIt.first != posEdgesMapIt.second) {
            while (posEdgesMapIt.first != posEdgesMapIt.second) {
                if (posEdgesMapIt.first->second ==
                        partOfSpeech) {
                    alreadyAdded = true;
                    break;
                }
                posEdgesMapIt.first ++;
            }
        }
        if (! alreadyAdded) {
            AnnotationItem ai(partOfSpeech);
            lattice.getAnnotationItemManager().setValue(ai, "discard", "0");
            lattice.getAnnotationItemManager().setValue(ai, "head", "0");
            Lattice::EdgeSequence::Builder seqBuilder(lattice);
            seqBuilder.addEdge(edge);
            lattice.addEdge(
                    start,
                    end,
                    ai,
                    lattice.getLayerTagManager().
                    createSingletonTagCollection("parse"),
                    seqBuilder.build()
                    );
            posEdgesMap.insert(std::pair< Lattice::VertexDescriptor, std::string>(
                        start, partOfSpeech));
        }
    }
}

std::string RuleMatcher::generateSentenceString(Lattice &lattice, int startVertex) {
    std::stringstream ss;
    if (startVertex == 0)
        ss << "<<s<0<0<sb<>";

    Lattice::VertexDescriptor vertex = lattice.getVertexForRawCharIndex(startVertex);
    while (vertex < lattice.getLastVertex() ) {
        std::list<Lattice::EdgeDescriptor> edges = lattice::getTopEdges(
                lattice, vertex);

        if (edges.empty()) {
            vertex ++;
            continue;
        }

        Lattice::EdgeDescriptor edge = edges.front();

        LayerTagCollection tags = lattice.getEdgeLayerTags(edge);
        LayerTagMask mask = lattice.getLayerTagManager().getMask(tags);
        if (lattice.getLayerTagManager().match(mask, "form") ||
                lattice.getLayerTagManager().match(mask, "token")) {
            ss << "<<t";
        } else {
            ss << "<<g";
        }

        Lattice::VertexDescriptor start = lattice.getEdgeBeginIndex(edge);
        Lattice::VertexDescriptor end = start + lattice.getEdgeLength(edge);
        ss << "<" << start;
        ss << "<" << end;
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);
        if (lattice.getLayerTagManager().match(mask, "parse")) {
            ss << "<" << lattice.getAnnotationItemManager().
                getCategory(annotationItem);
            std::string orth = lattice.getAnnotationItemManager().
                getValue(annotationItem, "orth");
            if (orth != "") {
            ss << "<" << util::escapeSpecialChars(orth);
            } else {
                ss << "<" << lattice.getAnnotationItemManager().
                getCategory(annotationItem);
            }
        } else {
            ss << "<" << "TOKEN";
            std::string orth = //lattice.getAnnotationItemManager().
                //getCategory(annotationItem);
                ////@todo: trzeba poprawic ustawianie orth dla krawedzi 'parse'
                lattice.getEdgeText(edge);
            ss << "<" << util::escapeSpecialChars(orth);
        }
        for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt = edges.begin();
                edgeIt != edges.end(); ++ edgeIt) {
            AnnotationItem ai = lattice.getEdgeAnnotationItem(*edgeIt);
            if (lattice::isDiscarded(lattice, *edgeIt))
                continue; //skip discarded edges
            std::string base = lattice::getBase(lattice, *edgeIt);
            //@todo: ustawianie base nie bedzie dzialalo dla krawedzi 'parse'
            std::string morphology = lattice::getMorphologyString(
                    lattice, *edgeIt);
            ss << "<";
            ss << util::escapeSpecialChars(base);
            ss << "<";
            ss << util::escapeSpecialChars(morphology);
        }
        ss << ">";
        vertex = end;
    }
    ss << "<<s<" << vertex << "<" << vertex << "<se<>";

    return ss.str();
}

void RuleMatcher::initProperties() {
    syntok = true;
    norepeats = false;
}


}

}

}

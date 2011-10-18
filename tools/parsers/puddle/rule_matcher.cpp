
#include "rule_matcher.hpp"

#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {


RuleMatcher::RuleMatcher() {
    syntok = true;
    disamb = false;
    norepeats = false;
}

RuleMatcher::~RuleMatcher() { }

void RuleMatcher::setRules(RulesPtr aRules) {
    rules = aRules;
}

void RuleMatcher::applyRules(Lattice &lattice) {
    Rules::iterator ir = rules->begin();

    while (ir != rules->end()) {
        std::string sentenceString = generateSentenceString(lattice);
        int currentEntity;
        std::string before = "";
        std::string after = "";
        int afterIndex = 0;
        std::string prev_before = "";
        std::vector<StringPiece> match;
        std::string tmpSentenceString = sentenceString;
        while ( (currentEntity = (*ir)->matchPattern(tmpSentenceString,
                        before, after, afterIndex, match) ) > -1 )
        {
            prev_before = before;
            bool structureChanged = false;
            RuleTokenSizes ruleTokenSizes;
            std::string oldSentenceString = sentenceString;
            if ( (*ir)->test(sentenceString, lattice, currentEntity, match,
                        ruleTokenSizes) ) {
                if ((*ir)->apply(sentenceString, lattice, currentEntity,
                            ruleTokenSizes)) {
                    sentenceString = generateSentenceString(lattice, currentEntity);
                    structureChanged = true;
                }
            }
            tmpSentenceString = generateSentenceString(lattice,
                    afterIndex);
            if ((*ir)->getRepeat()) {
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
        ++ ir;
    }

    unescapeSpecialChars(lattice);

    //@todo: dorobic robienie tych ponizszych paru sztuczek tez na latice

//    ParseGraph::Graph *g = inputGraph->getBoostGraph();
//    ParseGraph::TransitionMap map = boost::get(boost::edge_bundle, *g);
//
//    //@todo: te ponizsze super operacje nie sa na razie robione. wydumac, o co tam chodzilo i dorobic to na grafie
//    //for (Edges::iterator e = lattice.begin(); e != lattice.end(); e++)
//    //{
//    for (std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> eit = boost::edges(*g);
//            eit.first != eit.second; eit.first++) {
//        ParseGraph::Edge e = *eit.first;
//
//        //if (!(*e)->isLexical())
//        if (!map[e].isLexical())
//        //if ((*e)->isPhrasal())
//            continue;
//
//        bool lowFlag = false;
//        //std::string id = (*e)->getId();
//        std::string id = map[e].getId();
//        //int start = (*e)->getStart();
//        int start = map[e].getStart();
//        //int end = (*e)->getEnd();
//        int end = map[e].getEnd();
//
//        //std::string lowcase = (*e)->getLabel();
//        std::string lowcase = map[e].getLabel();
//        boost::to_lower(lowcase);
////        UnicodeString valL = icuConverter::fromUtf((*e)->getLabel());
////        valL.toLower();
////        StringCharacterIterator itL(valL);
////        std::stringstream ss;
////        while (itL.hasNext())
////        {
////            UnicodeString tl = itL.current();
////            ss << icuConverter::toUtf(tl);
////            itL.next();
////        }
////        lowcase = ss.str();
//        //if (lowcase != (*e)->getLabel())
//        if (lowcase != map[e].getLabel())
//            lowFlag = true;
//
//        if (lowFlag)
//        {
//            //for (int vari = 0; vari < (*e)->variants_.size(); vari ++)
//            for (int vari = 0; vari < map[e].variants_.size(); vari ++) {
//                //std::string var_base = boost::get<0>((*e)->variants_[vari]);
//                std::string var_base = boost::get<0>(map[e].variants_[vari]);
//
//                std::string lowbase = var_base;
//                boost::to_lower(lowbase);
////                UnicodeString valL = icuConverter::fromUtf(var_base);
////                valL.toLower();
////                StringCharacterIterator itL(valL);
////                std::stringstream ss;
////                while (itL.hasNext())
////                {
////                    UnicodeString tl = itL.current();
////                    ss << icuConverter::toUtf(tl);
////                    itL.next();
////                }
////                lowbase = ss.str();
//                if (lowbase == var_base) //istnieje forma bazowa pisana mala litera
//                {
//                    //(*e)->setLabel(lowcase);
//                    map[e].setLabel(lowcase);
//                    break;
//                }
//            }
//        }
//
//    }
//
//    ParseGraphPtr pg = ParseGraphPtr(new ParseGraph());
//
//    if (!syntok)
//    {
//        std::map<int, int> begins_mapped;
//        //for (Edges::iterator e = lattice.begin(); e != lattice.end(); e ++)
//        //{
//        for (std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> eit = boost::edges(*g);
//                eit.first != eit.second; eit.first++) {
//            ParseGraph::Edge e = *eit.first;
//
//                //if ((*e)->getType() == "group" && (*e)->getLabel() == "SYNTOK")
//                if (map[e].getType() == "group" && map[e].getLabel() == "SYNTOK")
//                {
//                    //std::map<int, int>::iterator it = begins_mapped.find((*e)->getStart());
//                    std::map<int, int>::iterator it = begins_mapped.find(map[e].getStart());
//                    if (it != begins_mapped.end())
//                    {
//                        //begins_mapped[(*e)->getStart()] = it->second + 1;
//                        begins_mapped[ map[e].getStart() ] = it->second + 1;
//                    }
//                    else
//                    {
//                        //begins_mapped.insert(std::pair<int, int>((*e)->getStart(), 1));
//                        begins_mapped.insert(std::pair<int, int>(map[e].getStart(), 1));
//                    }
//                    //if (((*e)->getEnd() - (*e)->getStart()) > 1)
//                    //{
//                    //e ++;
//                    //}
//                }
//        }
//
//        //for (Edges::iterator e = lattice.begin(); e != lattice.end(); e ++)
//        //{
//        for (std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> eit = boost::edges(*g);
//                eit.first != eit.second; eit.first++) {
//            ParseGraph::Edge e = *eit.first;
//            //if ((*e)->getType() == "group" && (*e)->getLabel() == "SYNTOK")
//            if (map[e].getType() == "group" && map[e].getLabel() == "SYNTOK")
//                continue;
//            //std::map<int, int>::iterator it = begins_mapped.find((*e)->getStart());
//            std::map<int, int>::iterator it = begins_mapped.find(map[e].getStart());
//            if (it != begins_mapped.end())
//            {
//                //(*e)->setDepth((*e)->getDepth() - it->second);
//                map[e].setDepth(map[e].getDepth() - it->second);
//            }
//
//            //pg->add_edge((*e)->getStart(), (*e)->getEnd(), **e);
//            pg->add_edge(map[e].getStart(), map[e].getEnd(), map[e]);
//        }
//    }
//    else
//    {
//        //for (Edges::iterator e = lattice.begin(); e != lattice.end(); e ++)
//        for (std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> eit = boost::edges(*g);
//                eit.first != eit.second; eit.first++) {
//            ParseGraph::Edge e = *eit.first;
//            //pg->add_edge((*e)->getStart(), (*e)->getEnd(), **e);
//            pg->add_edge(map[e].getStart(), map[e].getEnd(), map[e]);
//        }
//    }
////    e ++;

}

void RuleMatcher::unescapeSpecialChars(Lattice &) const {

    //@todo: dorobic te ponizsze rzeczy na Lattice, jak juz bedzie rzeczywiscie mozna zmieniac krawedzie

//    ParseGraph::Graph *g = pg->getBoostGraph();
//    ParseGraph::TransitionMap map = boost::get(boost::edge_bundle, *g);
//
//    //for (Edges::iterator e = edges.begin(); e != edges.end(); e++) {
//    for (std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> eit = boost::edges(*g);
//            eit.first != eit.second; eit.first++) {
//        ParseGraph::Edge e = *eit.first;
//
//        //if (!(*e)->isLexical())
//        if (!map[e].isLexical())
//            continue;
//
//        //std::string label = (*e)->getLabel();
//        std::string label = map[e].getLabel();
//        label = util::unescapeSpecialChars(label);
//        //(*e)->setLabel(label);
//        map[e].setLabel(label);
//
//        //for (int vari = 0; vari < (*e)->variants_.size(); vari ++)
//        for (int vari = 0; vari < map[e].variants_.size(); vari ++) {
//            //std::string var_base = boost::get<0>((*e)->variants_[vari]);
//            std::string var_base = boost::get<0>(map[e].variants_[vari]);
//            var_base = util::unescapeSpecialChars(var_base);
//            //if (var_base != boost::get<0>((*e)->variants_[vari]))
//            if (var_base != boost::get<0>(map[e].variants_[vari])) {
//                //PosInfo new_var = PosInfo(var_base, boost::get<1>((*e)->variants_[vari]), boost::get<2>((*e)->variants_[vari]));
//                PosInfo new_var = PosInfo(var_base,
//                        boost::get<1>(map[e].variants_[vari]),
//                        boost::get<2>(map[e].variants_[vari]));
//                //(*e)->variants_[vari] = new_var;
//                map[e].variants_[vari] = new_var;
//            }
//        }
//    }
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
        //if (lattice.getAnnotationItemManager().getValue(annotationItem,
        //            "discard") == "1")
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
                    //                                0.0, //@todo: jaki tu score?
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
        if (lattice.getLayerTagManager().match(mask, "form")) {
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
            ss << "<" << orth;
            } else {
                ss << "<" << lattice.getAnnotationItemManager().
                getCategory(annotationItem);
            }
        } else {
            ss << "<" << "TOKEN";
            ss << "<" << //lattice.getAnnotationItemManager().
                //getCategory(annotationItem); //@todo: trzeba poprawic ustawianie orth dla krawedzi 'parse'
                lattice.getEdgeText(edge);
        }
        for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt = edges.begin();
                edgeIt != edges.end(); ++ edgeIt) {
            AnnotationItem ai = lattice.getEdgeAnnotationItem(*edgeIt);
            //if (lattice.getAnnotationItemManager().getValue(ai, "discard") == "1")
            if (lattice::isDiscarded(lattice, *edgeIt))
                continue; //skip discarded edges
            std::string base = lattice::getBase(lattice, *edgeIt); //@todo: nie bedzie dzialalo dla krawedzi 'parse'
            //std::string partOfSpeech = lattice::getPartOfSpeech(lattice, *edgeIt); //@todo: nie bedzie dzialalo dla krawedzi 'parse'
            //std::string morpho = lattice.getAnnotationItemManager().getValue(
            //       ai, "morpho");
            //std::string morphology = partOfSpeech;
            //if (morpho != "")
            //    morphology += ":" + morpho;
            std::string morphology = lattice::getMorphologyString(
                    lattice, *edgeIt);
            ss << "<";
            ss << base;
            ss << "<";
            ss << morphology;
        }
        ss << ">";
        vertex = end;
    }
    ss << "<<s<" << vertex << "<" << vertex << "<se<>"; //@todo: tu taki ma byc ten koniec? czy vertex + 1? chyba vertex jednak

    return ss.str();
}

void RuleMatcher::setSyntok() {
    syntok = true;
}

void RuleMatcher::setNoSyntok() {
    syntok = false;
}

void RuleMatcher::setDisamb() {
    disamb = true;
}

void RuleMatcher::setNoRepeats() {
    norepeats = true;
}

}

}

}

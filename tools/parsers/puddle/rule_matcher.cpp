
#include "rule_matcher.hpp"

#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {


RuleMatcher::RuleMatcher() {
//    rules = new Rules;
    syntok = true;
    disamb = false;
    norepeats = false;
}

RuleMatcher::~RuleMatcher() {
//    delete rules;
}

//void RuleMatcher::setRules(Rules aRules)
void RuleMatcher::setRules(RulesPtr aRules) {
//    delete rules;
    //rules = //new Rules(aRules);
    rules = aRules;
}

//ParseGraphPtr RuleMatcher::applyRules(std::string &sentence, Entities &entities, Edges &lattice)
//ParseGraphPtr RuleMatcher::applyRules(std::string &sentenceString, ParseGraphPtr inputGraph, Lattice &lattice)
void RuleMatcher::applyRules(std::string &sentenceString,
        Lattice &lattice) {
//    std::cerr << "ZDANIE!" << std::endl;
    //trzeba jechać po kolei tokenami ze zdania - zdanie to nie bedzie string, tylko obiekt: co najmniej wektor tokenow (elementow tych od grafu) oraz napis bedacy tym 'skompilowanym' ciagiem
    //dla pierwszego tokena, dla drugiego, etc, zrob: ...
    //

//    std::cerr << sentence << std::endl;

    //Entities::iterator it = entities.begin();
    //int currentEntity = 0;
    //while (1)
    //{
    //bool anything = false;

    Rules::iterator ir = rules->begin();

    while (ir != rules->end())
    //for (Rules::iterator ir = rules.begin(); ir != rules.end(); ir ++)
    {
//        std::cerr << "Regula: " << (*ir)->getName() << std::endl;
        int currentMatch = 0;
        int currentEntity;
        bool first_match = true;
        std::string before = "";
        std::string prev_before = "";
        std::vector<re2::StringPiece> match;
        //@todo: przerobic to tak, zeby nie bylo miliard razy dopasowywane ten lancuch, tylko zeby go dopasowac tyle razy ile pasuje i odpowiednio odpalac reguly tam, gdzie pasuja
        //while ((currentEntity = (*ir)->matchPattern(sentence, currentMatch, before)) > -1)
        while ( (currentEntity = (*ir)->matchPattern(sentenceString,
                        currentMatch, before, match) ) > -1 )
        {
            currentMatch ++;

            //OLD:
            //if (norepeats)
            //    if (prev_before == before)
            //        continue;
            if (!first_match)
            {
                if (!(*ir)->getRepeat())
                {
                    if (prev_before == before)
                        continue;
                }
            }
            first_match = false;
                    prev_before = before;
//                std::cerr << "Pasuje!" << std::endl;
//       std::cerr << "Zdanie: " << sentence << std::endl;
        bool structureChanged = false;
        //int currentEntity = 0;
        //for (Entities::iterator it = entities.begin(); it != entities.end(); it ++)
       // {
        //std::string oldSentence = sentence;
        std::string oldSentenceString = sentenceString;
        //if ((*ir)->test(sentence, entities, currentEntity))
        //if ((*ir)->test(sentenceString, inputGraph, currentEntity))
        if ( (*ir)->test(sentenceString, lattice, currentEntity, match) ) {
//                std::cerr << "test zdany" << std::endl;
                //if ((*ir)->apply(sentence, entities, lattice, currentEntity))
                //if ((*ir)->apply(sentenceString, inputGraph, lattice, currentEntity))
                if ((*ir)->apply(sentenceString, lattice, currentEntity)) {
                    //sentence = generateSentencePattern(lattice);
                    //sentenceString = generateSentenceString(inputGraph, lattice);
//                    std::cerr << "SENT PRZED: " << oldSentenceString << std::endl;
                    sentenceString = generateSentenceString(lattice);
//                    std::cerr << "SENT PO:    " << sentenceString << std::endl;
                    //TU: na nowo zrobic sentence
//                    prev_before = before;
//                    std::cerr << "zaaplikowany" << std::endl;
                    //if (sentence != oldSentence)
                    if (sentenceString != oldSentenceString)
                        currentMatch --;
                    structureChanged = true;
              //      break;
                }
            }

        //    currentEntity ++;
        //}

        if (structureChanged)
        {
        //    ir = rules.begin();
        //    currentMatch = 0;
            continue;
        }
        }
//        std::cerr << "koniec reguly" << std::endl;
        ++ ir;
    }

//    std::cerr << "reguly zrobione" << std::endl;
    //unescapeSpecialChars(lattice);
    //unescapeSpecialChars(inputGraph, lattice);
    unescapeSpecialChars(lattice);
//    std::cerr << "spec-znaki wyrzucone" << std::endl;

    //addPosEdges(lattice);
    //addPosEdges(inputGraph, lattice);
//    addPosEdges(lattice);
//    std::cerr << "pos krawedzie dodane" << std::endl;

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

//    return pg;
    //if (!anything)
    //    break;
    //}
}

//void RuleMatcher::unescapeSpecialChars(Edges &edges) {
//void RuleMatcher::unescapeSpecialChars(ParseGraphPtr pg, Lattice &lattice) {
void RuleMatcher::unescapeSpecialChars(Lattice &) {

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

//void RuleMatcher::addPosEdges(Edges &edges) {
//void RuleMatcher::addPosEdges(ParseGraphPtr pg, Lattice &lattice) {
void RuleMatcher::addPosEdges(Lattice &lattice) {
    //Edges posEdges;
    LayerTagMask mask = lattice.getLayerTagManager().getMask(
            lattice.getLayerTagManager().
            createSingletonTagCollection("form") //@todo: czy "token"?
            );

    //std::multimap<Lattice::EdgeDescriptor, std::string> posEdgesMap;
    std::multimap<Lattice::VertexDescriptor, std::string> posEdgesMap;
    Lattice::EdgesSortedBySourceIterator edgeIterator =
        lattice.edgesSortedBySource(mask);
    while (edgeIterator.hasNext()) {
        Lattice::EdgeDescriptor edge = edgeIterator.next();
        int start = lattice.getEdgeBeginIndex(edge);
        int end = start + lattice.getEdgeLength(edge);
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);
        if (lattice.getAnnotationItemManager().getValue(annotationItem,
                    "discard") == "1")
            continue; //skip discarded forms
        //std::string morphology =
        //    lattice.getAnnotationItemManager().getValue(ai, "morphology");
        //std::string partOfSpeech = morphology.substr(0, morphology.find(":"));
        std::string partOfSpeech = lattice::getPartOfSpeech(lattice, edge);
        //                    std::list< std::pair<std::string, std::string> > av
        //                        = lattice.getAnnotationItemManager().getValues(ai);
        //                    for (std::list< std::pair<std::string, std::string> >
        //                            ::iterator avi = av.begin();
        //                            avi != av.end(); avi ++) {
        //                        if (avi->first == "pos")
        //                            partOfSpeech = avi->second;
        //                    }
        std::pair<
            //std::multimap<Lattice::EdgeDescriptor, std::string>::iterator,
            //std::multimap<Lattice::EdgeDescriptor, std::string>::iterator
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
            Lattice::EdgeSequence::Builder seqBuilder;
            seqBuilder.addEdge(edge);
            lattice.addEdge(
                    start,
                    end,
                    ai,
                    lattice.getLayerTagManager().
                    createSingletonTagCollection("parse"), //@todo: czy cos innego jeszcze? a moze lemma?
                    //                                0.0, //@todo: jaki tu score?
                    seqBuilder.build()
                    );
            posEdgesMap.insert(std::pair< Lattice::VertexDescriptor, std::string>(
                        start, partOfSpeech));
        }
    }

//    ParseGraph::Graph *g = pg->getBoostGraph();
//    ParseGraph::TransitionMap map = boost::get(boost::edge_bundle, *g);
//    //for (Edges::iterator e = edges.begin(); e != edges.end(); e++) {
//    for (std::pair<ParseGraph::EdgeIt, ParseGraph::EdgeIt> eit = boost::edges(*g);
//            eit.first != eit.second; eit.first++) {
//        ParseGraph::Edge e = *eit.first;
//
//        //if (!(*e)->isLexical())
//        if (! map[e].isLexical())
//            continue;
//
//        //int start = (*e)->getStart();
//        int start = map[e].getStart();
//        //int end = (*e)->getEnd();
//        int end = map[e].getEnd();
//
//        std::set<std::string> parts;
//
//        //std::vector<PosInfo> vars = (*e)->variants_;
//        std::vector<PosInfo> vars = map[e].variants_;
//        //for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
//        for (std::vector<PosInfo>::iterator pi = map[e].variants_.begin();
//                pi != map[e].variants_.end(); pi ++) {
//            if (!boost::get<2>(*pi))
//                continue;
//            std::string morpho = boost::get<1>(*pi);
//            std::string part = morpho.substr(0, morpho.find(":", 1));
//            if (parts.find(part) == parts.end())
//            {
//                TransitionInfo *pos = new TransitionInfo("pos");
//                pos->setLabel(part);
//                pos->setDepth(1);
//                pos->setStart(start);
//                pos->setEnd(end);
//                //pos->variants_ = (*e)->variants_;
//                pos->variants_ = map[e].variants_;
//                //pos->setHead((*e)->getId());
//                pos->setHead(map[e].getId());
//
//                //@todo: generowanie tych identyfikatorow zmienic
//                //std::stringstream *ss = new std::stringstream;
//                //*ss << std::hex << Group::groupId;
//                //std::string pos_id = ss->str();
//                //delete ss;
//                pos->setId( util::getNewEdgeId(pg) );
//                //Group::groupId++;
//
//        //        posEdges.push_back(pos);
//                pg->add_edge(pos->getStart(), pos->getEnd(), *pos);
//                parts.insert(part);
//                if (!disamb)
//                    break;
//            }
//        }
//        if (parts.size() == 0) //gdy nie bylo zadnych interpretacji, bo wyrznely wszystko reguly
//        {
//            //if ((*e)->variants_.size() == 0)
//            if (map[e].variants_.size() == 0) {
//                //(*e)->addMorphology(PosInfo((*e)->getLabel(), "ign", 1));
//                map[e].addMorphology(PosInfo(map[e].getLabel(), "ign", 1));
//            }
//            //PosInfo pi = (*e)->variants_.front();
//            PosInfo pi = map[e].variants_.front();
//            std::string base = boost::get<0>(pi);
//            //(*e)->addMorphology(PosInfo(base, "ign", 1));
//            map[e].addMorphology(PosInfo(base, "ign", 1));
//            TransitionInfo *pos = new TransitionInfo("pos");
//            pos->setLabel("ign");
//            pos->setDepth(1);
//            //pos->setStart((*e)->getStart());
//            pos->setStart(map[e].getStart());
//            //pos->setEnd((*e)->getEnd());
//            pos->setEnd(map[e].getEnd());
//            //pos->setHead((*e)->getId());
//            pos->setHead(map[e].getId());
//            //pos->variants_ = (*e)->variants_;
//            pos->variants_ = map[e].variants_;
//
//      //      posEdges.push_back(pos);
//            pg->add_edge(pos->getStart(), pos->getEnd(), *pos);
//        }
//    }

    //for (Edges::iterator ei = posEdges.begin(); ei != posEdges.end(); ei++)
        //edges.push_back(*ei);
    //    pg->add_edge((*ei)->getStart(), (*ei)->getEnd(), **ei);

}

//std::string RuleMatcher::generateSentencePattern(Edges &edges) {
//std::string RuleMatcher::generateSentenceString(ParseGraphPtr pg, Lattice &lattice) {
std::string RuleMatcher::generateSentenceString(Lattice &lattice) {
/*    ParseGraphPtr tmp_pg = ParseGraphPtr(new ParseGraph()); //tymczasowy graf, wszak dzialamy docelowo na grafie/kracie, a nie na jakis badziewiach
    for (Edges::iterator e = edges.begin(); e != edges.end(); e ++) {
        tmp_pg->add_edge((*e)->getStart(), (*e)->getEnd(), **e);
    }*/
    std::stringstream ss;
    ss << "<<s<0<0<sb<>";

//    ParseGraph::Graph *g = pg->getBoostGraph();
//    //ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g); //potrzebne to w ogole?
//    ParseGraph::VertexIndex index = get(boost::vertex_index, *g);
//    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);
//
//    //std::string emptyMorphology(tagset->getNumberOfAttributes(), '0');
//    int i = 0;
//    while (i < (boost::num_vertices(*g) - 1) ) {
//    //for (int i = 0; i < boost::num_vertices(*g); i ++) {
//        ParseGraph::Vertex v = vertex(i, *g);
//
//        int max_depth = 0;
//        int max_start = -1;
//        int max_end = -1;
//        std::string max_label, max_type, max_orth;
//        std::vector<PosInfo> max_variants;
//
//        for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) {
//            ParseGraph::Edge e = *p.first;
//            if (map[e].isPos())
//                continue;
//            if ((max_depth == 0) || ((max_depth > 0) && (map[e].getDepth() > max_depth))) {
//                max_depth = map[e].getDepth();
//                max_start = map[e].getStart();
//                max_end = map[e].getEnd();
//                max_label = map[e].getLabel();
//                max_type = map[e].getType();
//                max_orth = map[e].getOrth();
//                for (std::vector<PosInfo>::iterator var_it = map[e].variants_.begin();
//                        var_it != map[e].variants_.end(); var_it ++) {
//                    if (boost::get<2>(*var_it))
//                        max_variants.push_back(*var_it);
//                }
//                //max_variants = map[e].variants_;
//            }
//        }
//        if (max_start == -1) {
//            i ++;
//            continue;
//        }
//
//        if (max_type != "group") {
//            ss << "<<t";
//        } else {
//            ss << "<<g";
//        }
//        ss << "<" << max_start;
//        ss << "<" << max_end;
//        if (max_type == "group") {
//            ss << "<" << max_label;
//        } else {
//            ss << "<" << "TOKEN";
//        }
//        ss << "<" << max_orth; //@todo: nie dziala to dla gruyp
//        if (max_variants.size() == 0) {
//            PosInfo pi(max_orth, "ign", 1); //@todo: ten identyfikator ign ma byc konfigurowalny
//            max_variants.push_back(pi);
//        }
//        for (std::vector<PosInfo>::iterator vit = max_variants.begin();
//                vit != max_variants.end(); vit ++) {
//            if (! boost::get<2>(*vit))
//                continue;
//            //std::string mapped = tagset->mapMorphology(boost::get<1>(*vit));
//            //if (mapped == "")
//            //    mapped = emptyMorphology;
//            ss << "<";
//            //ss << mapped;
//            ss << boost::get<0>(*vit);
//            ss << "<";
//            ss << boost::get<1>(*vit);
//        }
//        ss << ">";
//        i = max_end;
//    }
//    ss << "<<s<" << i - 1 << "<" << i - 1 << "<se<>";

    Lattice::VertexDescriptor vertex = lattice.getFirstVertex();
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
            if (lattice.getAnnotationItemManager().getValue(ai, "discard") == "1")
                continue; //skip discarded edges
            std::string base = lattice::getBase(lattice, *edgeIt); //@todo: nie bedzie dzialalo dla krawedzi 'parse'
                //lattice.getAnnotationItemManager().getValue(
                    //ai, "base");
            std::string partOfSpeech = lattice::getPartOfSpeech(lattice, *edgeIt); //@todo: nie bedzie dzialalo dla krawedzi 'parse'
            std::string morpho = lattice.getAnnotationItemManager().getValue(
                    ai, "morpho");
            std::string morphology = partOfSpeech;
            if (morpho != "")
                morphology += ":" + morpho;
            ss << "<";
            //ss << mapped;
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

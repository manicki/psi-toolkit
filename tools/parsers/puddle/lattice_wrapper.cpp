#include "lattice_wrapper.hpp"

namespace poleng {

    namespace bonsai {
        namespace puddle {

            namespace lattice {

                struct SentenceToken {
                    std::string orth;
                    std::string category;
                    int start;
                    int end;
                    std::vector<std::string> baseForms;
                    std::vector<std::string> morphology;
                    std::string type;
                };

                std::string readMorfologikLattice(Lattice &lattice) {
                    std::string sentenceString = "";

                    std::map<int, SentenceToken> edgesMap;

                    LayerTagCollection form_tag
                        = lattice.getLayerTagManager().createSingletonTagCollection("form");
                    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
                    LayerTagCollection parse_tag
                        = lattice.getLayerTagManager().createSingletonTagCollection("parse");
                    LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
                    LayerTagMask mask = lattice.getLayerTagManager().getMask(
                            createUnion(form_tag, parse_tag) );

                    Lattice::EdgesSortedBySourceIterator edgeIterator =
                        lattice.edgesSortedBySource(mask);
                    while (edgeIterator.hasNext()) {
                        Lattice::EdgeDescriptor edge = edgeIterator.next();
                        LayerTagCollection tags = lattice.getEdgeLayerTags(edge);
                        std::list<std::string> tagNames =
                            lattice.getLayerTagManager().getTagNames(tags);
                        bool isParse = false;
                        for (std::list<std::string>::iterator tagIt = tagNames.begin();
                                tagIt != tagNames.end(); ++ tagIt) {
                            if (*tagIt == "parse") {
                                isParse = true;
                                break;
                            }
                        }
                        int start = lattice.getEdgeBeginIndex(edge);
                        int end = start + lattice.getEdgeLength(edge);
                        std::string orth = lattice.getEdgeText(edge);
                        //@todo: przerabiac to na wygrzebywaine tej formy z krawedzi 'token'
                        //czy zostawic jak jest?
                        AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                        std::string category = ai.getCategory();
                        std::list< std::pair<std::string, std::string> > av
                            = lattice.getAnnotationItemManager().getValues(ai);
                        //if (lattice.getAnnotationItemManager().getValue(
                        //           ai, "discard") == "1")
                        //    continue; //skip discarded edges
                        std::string base = getBase(lattice, edge);
                        std::string morphology = getMorphologyString(lattice, edge);

                        std::map<int, SentenceToken>::iterator edgesMapIt =
                            edgesMap.find(start);
                        if (edgesMapIt != edgesMap.end()) {
                            if (isParse) {
                                if (edgesMapIt->second.type == "parse") {
                                    edgesMapIt->second.baseForms.push_back(base);
                                    edgesMapIt->second.morphology.push_back(morphology);
                                } else {
                                    edgesMapIt->second.baseForms.clear();
                                    edgesMapIt->second.morphology.clear();
                                    edgesMapIt->second.type = "parse";
                                    edgesMapIt->second.category = category;
                                    edgesMapIt->second.baseForms.push_back(base);
                                    edgesMapIt->second.morphology.push_back(morphology);
                                }
                            } else {
                                if (edgesMapIt->second.type == "form") {
                                    edgesMapIt->second.baseForms.push_back(base);
                                    edgesMapIt->second.morphology.push_back(morphology);
                                }
                            }
                        } else {
                            SentenceToken edge;
                            edge.orth = orth;
                            edge.category = category;
                            edge.start = start;
                            edge.end = end;
                            edge.baseForms.push_back(base);
                            edge.morphology.push_back(morphology);
                            if (isParse)
                                edge.type = "parse";
                            else
                                edge.type = "form";
                            edgesMap.insert(std::pair<int, SentenceToken>(
                                        start, edge));
                        }

                    }

                    int start = 0;
                    int end = 0;
                    std::stringstream ss;
                    ss << "<<s<" << start << "<" << start << "<sb<>";
                    for (std::map<int, SentenceToken>::iterator edgesMapIt =
                            edgesMap.begin();
                            edgesMapIt != edgesMap.end(); ++ edgesMapIt) {
                        start = edgesMapIt->second.start;
                        end = edgesMapIt->second.end;
                        std::string type = edgesMapIt->second.type;
                        ss << "<<";
                        if (type == "form")
                            ss << "t";
                        else
                            ss << "g";
                        ss << "<" << start << "<" << end << "<";
                        if (type == "form")
                            ss << "TOKEN";
                        else
                            ss << edgesMapIt->second.category;
                        ss << "<" << edgesMapIt->second.orth;
                        std::vector<std::string>::iterator baseIt =
                            edgesMapIt->second.baseForms.begin();
                        std::vector<std::string>::iterator morphIt =
                            edgesMapIt->second.morphology.begin();
                        while (baseIt != edgesMapIt->second.baseForms.end()) {
                            ss << "<" << *baseIt << "<" << *morphIt;
                            ++ baseIt;
                            ++ morphIt;
                        }
                        ss << ">";
                    }
                    ss << "<<s<" << end << "<" << end << "<se<>";
                    sentenceString = ss.str();
                    return sentenceString;;
                }

                std::string readInputLattice(Lattice &lattice) {
                    std::string sentenceString = "";

                    std::map<int, SentenceToken> edgesMap;

                    LayerTagCollection form_tag
                        = lattice.getLayerTagManager().createSingletonTagCollection("form");
                    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
                    LayerTagCollection parse_tag
                        = lattice.getLayerTagManager().createSingletonTagCollection("parse");
                    LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
                    LayerTagMask mask = lattice.getLayerTagManager().getMask(
                            createUnion(form_tag, parse_tag) );

                    Lattice::EdgesSortedBySourceIterator edgeIterator =
                        lattice.edgesSortedBySource(mask);
                    while (edgeIterator.hasNext()) {
                        Lattice::EdgeDescriptor edge = edgeIterator.next();
                        LayerTagCollection tags = lattice.getEdgeLayerTags(edge);
                        std::list<std::string> tagNames =
                            lattice.getLayerTagManager().getTagNames(tags);
                        bool isParse = false;
                        for (std::list<std::string>::iterator tagIt = tagNames.begin();
                                tagIt != tagNames.end(); ++ tagIt) {
                            if (*tagIt == "parse") {
                                isParse = true;
                                break;
                            }
                        }
                        int start = lattice.getEdgeBeginIndex(edge);
                        int end = start + lattice.getEdgeLength(edge);
                        std::string orth = lattice.getEdgeText(edge);
                        //@todo: przerabiac to na wygrzebywaine tej formy z krawedzi 'token'
                        //czy zostawic jak jest?
                        AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                        std::string category = ai.getCategory();
                        std::list< std::pair<std::string, std::string> > av
                            = lattice.getAnnotationItemManager().getValues(ai);
                        if (isDiscarded(lattice, edge))
                            continue; //skip discarded edges
                        std::string base = getBase(lattice, edge);
                        std::string morphology = getMorphologyString(
                                lattice, edge);
                        //std::string morpho = lattice.getAnnotationItemManager().getValue(
                        //        ai, "morpho");
                        //std::string morphology = partOfSpeech;
                        //if (morpho != "")
                        //    morphology += ":" + morpho;

                        std::map<int, SentenceToken>::iterator edgesMapIt =
                            edgesMap.find(start);
                        if (edgesMapIt != edgesMap.end()) {
                            if (isParse) {
                                if (edgesMapIt->second.type == "parse") {
                                    edgesMapIt->second.baseForms.push_back(base);
                                    edgesMapIt->second.morphology.push_back(morphology);
                                } else {
                                    edgesMapIt->second.baseForms.clear();
                                    edgesMapIt->second.morphology.clear();
                                    edgesMapIt->second.type = "parse";
                                    edgesMapIt->second.category = category;
                                    edgesMapIt->second.baseForms.push_back(base);
                                    edgesMapIt->second.morphology.push_back(morphology);
                                }
                            } else {
                                if (edgesMapIt->second.type == "form") {
                                    edgesMapIt->second.baseForms.push_back(base);
                                    edgesMapIt->second.morphology.push_back(morphology);
                                }
                            }
                        } else {
                            SentenceToken edge;
                            edge.orth = orth;
                            edge.category = category;
                            edge.start = start;
                            edge.end = end;
                            edge.baseForms.push_back(base);
                            edge.morphology.push_back(morphology);
                            if (isParse)
                                edge.type = "parse";
                            else
                                edge.type = "form";
                            edgesMap.insert(std::pair<int, SentenceToken>(
                                        start, edge));
                        }

                    }

                    int start = 0;
                    int end = 0;
                    std::stringstream ss;
                    ss << "<<s<" << start << "<" << start << "<sb<>";
                    for (std::map<int, SentenceToken>::iterator edgesMapIt =
                            edgesMap.begin();
                            edgesMapIt != edgesMap.end(); ++ edgesMapIt) {
                        start = edgesMapIt->second.start;
                        end = edgesMapIt->second.end;
                        std::string type = edgesMapIt->second.type;
                        ss << "<<";
                        if (type == "form")
                            ss << "t";
                        else
                            ss << "g";
                        ss << "<" << start << "<" << end << "<";
                        if (type == "form")
                            ss << "TOKEN";
                        else
                            ss << edgesMapIt->second.category;
                        ss << "<" << edgesMapIt->second.orth;
                        std::vector<std::string>::iterator baseIt =
                            edgesMapIt->second.baseForms.begin();
                        std::vector<std::string>::iterator morphIt =
                            edgesMapIt->second.morphology.begin();
                        while (baseIt != edgesMapIt->second.baseForms.end()) {
                            ss << "<" << *baseIt << "<" << *morphIt;
                            ++ baseIt;
                            ++ morphIt;
                        }
                        ss << ">";
                    }
                    ss << "<<s<" << end << "<" << end << "<se<>";
                    sentenceString = ss.str();

                    return sentenceString;;
                }

#if _WITH_BONSAI_PARSEGRAPH
            ParseGraphPtr convertToBonsaiGraph(Lattice &lattice) {
                ParseGraphPtr pg = ParseGraphPtr(new ParseGraph());

                std::multimap<std::pair<int, int>, TransitionInfo*> edgesMap;
                std::map<int, int> depthsMap;

                LayerTagMask mask = lattice.getLayerTagManager().getMask(
                        createUnion(
                            createUnion(
                                lattice.getLayerTagManager().
                                createSingletonTagCollection("form"),
                                lattice.getLayerTagManager().
                                createSingletonTagCollection("parse")
                                ),
                            lattice.getLayerTagManager().
                            createSingletonTagCollection("lexeme") // former pos edge
                            )
                        );

                Lattice::EdgesSortedBySourceIterator edgeIterator =
                    lattice.edgesSortedBySource(mask);
                while (edgeIterator.hasNext()) {
                    Lattice::EdgeDescriptor edge = edgeIterator.next();
                    std::string type;
                    LayerTagMask edgeMask = lattice.getLayerTagManager().getMask(
                                lattice.getEdgeLayerTags(edge));
                    if (lattice.getLayerTagManager().match(edgeMask, "form"))
                        type = "token";
                    if (lattice.getLayerTagManager().match(edgeMask, "parse"))
                        type = "group";
                    if (lattice.getLayerTagManager().match(edgeMask, "lexeme"))
                        type = "pos";

                    int start = lattice.getEdgeBeginIndex(edge);
                    int end = start + lattice.getEdgeLength(edge);
                    std::pair<int, int> edgeCoord(start, end);
                    AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                    std::string category = ai.getCategory();

                    if (type == "token" || type == "group") {
                        std::string orth = lattice.getEdgeText(edge);
                        if (isDiscarded(lattice, edge)) {
                            continue; //skip discarded edges
                        }
                        std::string base = lattice::getBase(lattice, edge);
                        std::string partOfSpeech = getPartOfSpeech(lattice, edge);
                        //std::string morpho = lattice.getAnnotationItemManager().getValue(
                        //        ai, "morpho");
                        std::string morphology = getMorphologyString(lattice, edge);
                        //std::string morphology = partOfSpeech;
                        //if (morpho != "")
                        //    morphology += ":" + morpho;
                        PosInfo pi(base, morphology, 1);
                        if (type == "token") {
                            TransitionInfo *edge = new TransitionInfo("token");
                            edge->setStart(start);
                            edge->setEnd(end);
                            edge->setDepth(0);
                            edge->setLabel(orth);
                            edge->setOrth(orth);
                            edge->addMorphology(pi);
                             std::pair<
                                    std::map<std::pair<int, int>, TransitionInfo*>::iterator,
                                    std::map<std::pair<int, int>, TransitionInfo*>::iterator
                                        > edgesMapIt = edgesMap.equal_range(edgeCoord);
                             bool addNewEdge = true;
                             while (edgesMapIt.first != edgesMapIt.second) {
                                 if (edgesMapIt.first->second->getDepth() == 0) {
                                     edgesMapIt.first->second->addMorphology(pi);
                                     addNewEdge = false;
                                     break;
                                 }
                                 edgesMapIt.first ++;
                             }
                             if (addNewEdge) {
                                 edgesMap.insert(std::pair<std::pair<int, int>, TransitionInfo*>(
                                             edgeCoord, edge));
                             }
                        }

                        if (type == "group") {
                            std::map<int, int>::iterator depthsMapIt =
                                depthsMap.find(start);
                            int max_depth = 0;
                            if (depthsMapIt != depthsMap.end()) {
                                std::pair<
                                    std::map<std::pair<int, int>, TransitionInfo*>::iterator,
                                    std::map<std::pair<int, int>, TransitionInfo*>::iterator
                                        > edgesMapIt = edgesMap.equal_range(edgeCoord);
                                if (edgesMapIt.first == edgesMapIt.second) {
                                    max_depth = depthsMapIt->second + 1;
                                    TransitionInfo *edge = new TransitionInfo("group");
                                    edge->setStart(start);
                                    edge->setEnd(end);
                                    edge->setDepth(max_depth + 2);
                                    edge->setLabel(category);
                                    edge->setOrth(orth);
                                    edge->addMorphology(pi);
                                    edgesMap.insert(std::pair<std::pair<int, int>, TransitionInfo*>(
                                                edgeCoord, edge));
                                    depthsMap.insert(std::pair<int, int>(start, max_depth));
                                } else {
                                    bool groupFound = false;
                                    while (edgesMapIt.first != edgesMapIt.second) {
                                        if (edgesMapIt.first->second->getDepth() > max_depth)
                                            //@todo: po co tu szukanie tego max_depth?
                                            max_depth = edgesMapIt.first->second->getDepth();

                                        if (edgesMapIt.first->second->getLabel() == category) {
                                            //@todo:do czego ta morfologia jest naprawde doczepiana?
                                            edgesMapIt.first->second->addMorphology(pi);
                                            groupFound = true;
                                            break;
                                        }
                                        edgesMapIt.first ++;
                                    }
                                    if (! groupFound) {
                                        max_depth = depthsMapIt->second + 1;
                                        TransitionInfo *edge = new TransitionInfo("group");
                                        edge->setStart(start);
                                        edge->setEnd(end);
                                        edge->setDepth(max_depth + 2);
                                        edge->setLabel(category);
                                        edge->setOrth(orth);
                                        edge->addMorphology(pi);
                                        edgesMap.insert(std::pair<
                                                std::pair<int, int>, TransitionInfo*>(
                                                    edgeCoord, edge));
                                        depthsMap.insert(std::pair<int, int>(start, max_depth));
                                    }
                                }
                            } else {
                                TransitionInfo *edge = new TransitionInfo("group");
                                edge->setStart(start);
                                edge->setEnd(end);
                                edge->setDepth(max_depth + 2);
                                edge->setLabel(category);
                                edge->setOrth(orth);
                                edge->addMorphology(pi);
                                edgesMap.insert(std::pair<std::pair<int, int>, TransitionInfo*>(
                                            edgeCoord, edge));
                                depthsMap.insert(std::pair<int, int>(start, max_depth));
                            }

                        }
                    } else {
                        std::pair<
                            std::map<std::pair<int, int>, TransitionInfo*>::iterator,
                            std::map<std::pair<int, int>, TransitionInfo*>::iterator
                                > edgesMapIt = edgesMap.equal_range(edgeCoord);
                        bool addNewEdge = true;
                        //adds only one 'pos' edge to the parsegraph
                        while (edgesMapIt.first != edgesMapIt.second) {
                            if (edgesMapIt.first->second->getDepth() == 1) {
                                addNewEdge = false;
                                break;
                            }
                            edgesMapIt.first ++;
                        }
                        if (addNewEdge) {
                            TransitionInfo *edge = new TransitionInfo("pos");
                            edge->setStart(start);
                            edge->setEnd(end);
                            edge->setDepth(1);
                            edge->setLabel(category);
                            edgesMap.insert(std::pair<std::pair<int, int>, TransitionInfo*>(
                                        edgeCoord, edge));
                        }
                    }


                }
                for (std::map<std::pair<int, int>, TransitionInfo*>::iterator edgesMapIt =
                        edgesMap.begin();
                        edgesMapIt != edgesMap.end(); ++ edgesMapIt) {
                    int start = edgesMapIt->second->getStart();
                    int end = edgesMapIt->second->getEnd();
                    pg->add_edge(start, end, *(edgesMapIt->second));
                }

                return pg;
            }
#endif

            /*
             * returns start vertex descriptor for the edgeIndex-th edge in the lattice
             */
            Lattice::VertexDescriptor getVertex(Lattice &lattice,
                    int edgeIndex, int offset) {
                std::list<std::string> tags;
                tags.push_back("form");
                tags.push_back("parse");
                tags.push_back("token");
                LayerTagMask mask = lattice.getLayerTagManager().getMask(tags);
                return getVertex(lattice, edgeIndex, mask, offset);
            }

            Lattice::VertexDescriptor getVertex(Lattice &lattice,
                    int edgeIndex, LayerTagMask mask, int offset) {
                Lattice::VertexDescriptor vertex = offset;
                int edgePosition = 0;
                while (edgePosition < edgeIndex) {
                    Lattice::InOutEdgesIterator edgeIt =
                        lattice.outEdges(vertex, mask);
                    if (! edgeIt.hasNext()) {
                        vertex ++;
                        if (vertex == lattice.getLastVertex())
                            return lattice.getLastVertex();
                        continue;
                    }

                    Lattice::EdgeDescriptor edge;
                    while (edgeIt.hasNext())
                        edge = edgeIt.next();
                    if (isBlankTokenEdge(lattice, edge)) {
                        vertex ++;
                        if (vertex == lattice.getLastVertex())
                            return lattice.getLastVertex();
                        continue;
                    }

                    vertex = lattice.getEdgeBeginIndex(edge) +
                        lattice.getEdgeLength(edge);
                    if (vertex == lattice.getLastVertex())
                        return lattice.getLastVertex();
                    edgePosition ++;
                }
                Lattice::InOutEdgesIterator edgeIt =
                    lattice.outEdges(vertex, mask);
                while (! edgeIt.hasNext()) {
                    vertex ++;
                    if (vertex >= lattice.getLastVertex())
                        return lattice.getLastVertex();
                    edgeIt = lattice.outEdges(vertex, mask);
                }
                if (edgeIt.hasNext()) { //@todo: co jesli bedzie wiecej blank tokens niz 1 po sobie?
                    Lattice::EdgeDescriptor edge = edgeIt.next();
                    if (isBlankTokenEdge(lattice, edge)) {
                        vertex ++;
                        if (vertex == lattice.getLastVertex())
                            return lattice.getLastVertex();
                    }
                }
                return vertex;
            }

            std::list<Lattice::EdgeDescriptor> getTopEdges(
                    Lattice &lattice, Lattice::VertexDescriptor start) {
                std::list<std::string> tags;
                tags.push_back("form");
                tags.push_back("parse");
                tags.push_back("token");
                LayerTagMask mask = lattice.getLayerTagManager().getMask(tags);
                return getTopEdges(lattice, start, mask);
            }

            std::list<Lattice::EdgeDescriptor> getTopEdges(
                    Lattice &lattice, Lattice::VertexDescriptor start,
                    LayerTagMask mask) {
                std::list<Lattice::EdgeDescriptor> edges;
                std::list<Lattice::EdgeDescriptor> nontopEdges;
                Lattice::InOutEdgesIterator edgeIt =
                    lattice.outEdges(start, mask);
                bool nontokenEdgeFound = false;
                while (edgeIt.hasNext()) {
                    Lattice::EdgeDescriptor edge = edgeIt.next();
                    if (isDiscarded(lattice, edge))
                        continue;
                    if (isTokenEdge(lattice, edge)) {
                        if (isBlankTokenEdge(lattice, edge))
                            continue;
                    }
                    if (! isTokenEdge(lattice, edge))
                        nontokenEdgeFound = true;

                    edges.push_back(edge);
                    std::list<Lattice::Partition> partitions =
                        lattice.getEdgePartitions(edge);
                    for (std::list<Lattice::Partition>::iterator partIt =
                            partitions.begin();
                            partIt != partitions.end(); ++ partIt) {
                        Lattice::Partition::Iterator lpi(lattice, *partIt);
                        while (lpi.hasNext()) {
                            nontopEdges.insert(nontopEdges.end(), lpi.next());
                        }
                    }
                }

                nontopEdges.unique(EdgeUnique());
                edges.remove_if(EdgeNonTop(lattice, nontopEdges));
                if (nontokenEdgeFound) {
                    edges.remove_if(EdgeNonToken(lattice));
                }
                return edges;
            }

            std::list<Lattice::EdgeSequence> getEdgesRange(Lattice &lattice,
                    Lattice::VertexDescriptor start,
                    Lattice::VertexDescriptor end,
                    LayerTagMask mask) {
                std::list<Lattice::EdgeSequence> ranges;
                Lattice::VertexDescriptor vertex = start;
                while (vertex <= end) {
                    std::list<Lattice::EdgeDescriptor> edges = getTopEdges(lattice,
                            vertex, mask);
                    if (edges.empty()) {
                        vertex ++;
                        continue;
                    }
                    std::list<Lattice::EdgeSequence> tmpRanges;
                    if (! ranges.empty()) {
                        for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                                edges.begin(); edgeIt != edges.end(); ++ edgeIt) {
                            for (std::list<Lattice::EdgeSequence>::iterator seqIt =
                                    ranges.begin(); seqIt != ranges.end(); ++ seqIt) {
                                Lattice::EdgeSequence::Builder rangeBuilder(lattice);
                                Lattice::EdgeSequence::Iterator rangeIt(lattice, *seqIt);
                                while (rangeIt.hasNext()) {
                                    rangeBuilder.addEdge(rangeIt.next());
                                }
                                rangeBuilder.addEdge(*edgeIt);
                                tmpRanges.push_back(rangeBuilder.build());
                            }
                        }
                    } else {
                        for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                                edges.begin(); edgeIt != edges.end(); ++ edgeIt) {
                            Lattice::EdgeSequence::Builder rangeBuilder(lattice);
                            rangeBuilder.addEdge(*edgeIt);
                            tmpRanges.push_back(rangeBuilder.build());
                        }
                    }
                    ranges = tmpRanges;
                    vertex += lattice.getEdgeLength(edges.front());
                }
                return ranges;
            }

            std::list<Lattice::EdgeSequence> getEdgesRange(Lattice &lattice,
                    Lattice::VertexDescriptor start,
                    Lattice::VertexDescriptor end) {
                std::list<std::string> tags;
                tags.push_back("form");
                tags.push_back("parse");
                tags.push_back("token");
                LayerTagMask mask = lattice.getLayerTagManager().getMask(tags);
                return getEdgesRange(lattice, start, end, mask);
            }

            void addParseEdges(Lattice &lattice,
                    std::list<Lattice::EdgeDescriptor> startEdges,
                    std::list<Lattice::EdgeDescriptor> endEdges,
                    std::string &parseCategory,
                    std::list<Lattice::EdgeDescriptor> headEdges,
                    std::list<Lattice::EdgeSequence> groupSequences,
                    int headEdgeIndex,
                    Lattice::Score) {
                Lattice::VertexDescriptor startVertex =
                    lattice.getEdgeBeginIndex(startEdges.front());
                Lattice::VertexDescriptor endVertex =
                    lattice.getEdgeBeginIndex(endEdges.front()) +
                    lattice.getEdgeLength(endEdges.front());
                LayerTagCollection tags =
                    lattice.getLayerTagManager().createSingletonTagCollection("parse");
                for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                        headEdges.begin(); edgeIt != headEdges.end(); ++ edgeIt) {
                    AnnotationItem annotationItem(parseCategory);
                    std::list< std::pair<std::string, std::string> > values =
                        lattice.getAnnotationItemManager().getValues(
                                lattice.getEdgeAnnotationItem(*edgeIt) );
                    for (std::list< std::pair<std::string, std::string> >::iterator valueIt =
                            values.begin(); valueIt != values.end(); ++ valueIt) {
                        lattice.getAnnotationItemManager().setValue(
                                annotationItem, valueIt->first, valueIt->second);
                    }
                    lattice.getAnnotationItemManager().setValue(annotationItem, "orth",
                            lattice.getEdgeText(*edgeIt));
                    lattice.getAnnotationItemManager().setValue(annotationItem, "head",
                            boost::lexical_cast<std::string>(headEdgeIndex));

                    for (std::list<Lattice::EdgeSequence>::iterator seqIt =
                            groupSequences.begin();
                            seqIt != groupSequences.end();
                            ++ seqIt) {
                        if (sequenceContainsEdge(lattice, *seqIt, *edgeIt)) {
//                            bool firstPartition = true;
                            Lattice::InOutEdgesIterator outEdgesIt = lattice.outEdges(
                                    startVertex, lattice.getLayerTagManager().getMask(tags));
                            while (outEdgesIt.hasNext()) {
                                Lattice::EdgeDescriptor newEdge = outEdgesIt.next();
                                Lattice::VertexDescriptor readEnd = startVertex +
                                    lattice.getEdgeLength(newEdge);
                                if (readEnd != endVertex) {
                                    continue;
                                }
                                if (annotationItem ==
                                            lattice.getEdgeAnnotationItem(newEdge)) {
                                    //there is already such an edge. add another partition
                                    //@todo: to tak nie dziala. nowa partition powinno dodawac samo
                                    //addEdge, ale nie robi tego.
                                    //na razie jest wiec jedna partition zawsze

//                                    firstPartition = false;
                                    break;
                                }
                            }
                            //if (firstPartition) {
                            //there is no such an edge yet. add a new one
                            lattice.addEdge(
                                    startVertex,
                                    endVertex,
                                    annotationItem,
                                    tags,
                                    *seqIt
                                    );
                            //}
                        }
                    }
                }
            }

            void addSyntokEdges(Lattice &lattice,
                    std::list<Lattice::EdgeDescriptor> startEdges,
                    std::list<Lattice::EdgeDescriptor> endEdges,
                    std::string &syntokCategory,
                    std::string &concatenatedOrth,
                    std::vector<std::string> baseForms,
                    std::vector<Morphology> morphology,
                    std::list<Lattice::EdgeSequence> edgeSequences,
                    LayerTagCollection tags,
                    Lattice::Score) {

                Lattice::VertexDescriptor startVertex =
                    lattice.getEdgeBeginIndex(startEdges.front());
                Lattice::VertexDescriptor endVertex =
                    lattice.getEdgeBeginIndex(endEdges.front()) +
                    lattice.getEdgeLength(endEdges.front());

                for (std::vector<std::string>::iterator baseIt =
                        baseForms.begin();
                        baseIt != baseForms.end(); ++ baseIt) {

                    Lattice::EdgeDescriptor lemmaEdge;
                    if (! matchLemmaEdge(lattice, startVertex, *baseIt)) {
                        LayerTagCollection token_tag
                            = lattice.getLayerTagManager().createSingletonTagCollection("token");
                        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
                        Lattice::EdgeSequence::Builder lemmaBuilder(lattice);
                        lemmaBuilder.addEdge(lattice.firstOutEdge(startVertex, tokenMask));
                        AnnotationItem aiLemma("word", *baseIt);
                        LayerTagCollection tagsLemma = lattice.getLayerTagManager().
                            createSingletonTagCollection("lemma");
                        lemmaEdge = lattice.addEdge(
                                startVertex, endVertex, aiLemma, tagsLemma, lemmaBuilder.build()
                                );
                    } else
                        lemmaEdge = getLemmaEdge(lattice, startVertex, *baseIt);

                    for (std::vector<Morphology>::iterator morphIt =
                            morphology.begin();
                            morphIt != morphology.end();
                            ++ morphIt) {

                        std::string morphoString = util::getMorphologyString(
                                *morphIt);
                        size_t delimPos = morphoString.find(":");
                        std::string partOfSpeech = morphoString.substr(0, delimPos);
                        std::string lexeme = *baseIt + "_" + partOfSpeech;

                        Lattice::EdgeDescriptor lexemeEdge;
                        if (! matchLexemeEdge(lattice, startVertex,
                                    partOfSpeech, lexeme)) {
                            Lattice::EdgeSequence::Builder lexemeBuilder(lattice);
                            lexemeBuilder.addEdge(lemmaEdge);
                            AnnotationItem aiLexeme(partOfSpeech, StringFrag(lexeme));
                            LayerTagCollection tagsLexeme = lattice.getLayerTagManager().
                                createSingletonTagCollection("lexeme");
                            lexemeEdge = lattice.addEdge(
                                    startVertex, endVertex, aiLexeme, tagsLexeme,
                                    lexemeBuilder.build() );
                        } else
                            lexemeEdge = getLexemeEdge(lattice, startVertex,
                                    partOfSpeech, lexeme);

                        if (syntokCategory != concatenatedOrth) { //adding parse edge 'SYNTOK'
                            AnnotationItem annotationItem(syntokCategory);
                            //lattice.getAnnotationItemManager().setValue(
                            //        annotationItem, "morpho", morpho);
                            for (Morphology::iterator attribIt = morphIt->begin();
                                    attribIt != morphIt->end(); ++ attribIt) {
                                if (attribIt->first == "pos")
                                    continue;
                                lattice.getAnnotationItemManager().setValue(
                                        annotationItem, attribIt->first, attribIt->second);
                            }
                            lattice.getAnnotationItemManager().setValue(
                                    annotationItem, "orth", concatenatedOrth);
                            lattice.getAnnotationItemManager().setValue(
                                    annotationItem, "head", "0");
                            //@todo: czy tego tu inaczej jakos nie trzeba zrobic

                            for (std::list<Lattice::EdgeSequence>::iterator sequenceIt =
                                    edgeSequences.begin();
                                    sequenceIt != edgeSequences.end();
                                    ++ sequenceIt) {

                                lattice.addEdge(
                                        startVertex,
                                        endVertex,
                                        annotationItem,
                                        tags,
                                        *sequenceIt
                                        );
                            }

                        } else { //adding a 'form' edge
                            Lattice::EdgeSequence::Builder formBuilder(lattice);
                            formBuilder.addEdge(lexemeEdge);
                            AnnotationItem annotationItem(partOfSpeech, StringFrag(lexeme));
                            //lattice.getAnnotationItemManager().setValue(
                            //        annotationItem, "morpho", morpho);
                            for (Morphology::iterator attribIt = morphIt->begin();
                                    attribIt != morphIt->end(); ++ attribIt) {
                                if (attribIt->first == "pos")
                                    continue;
                                lattice.getAnnotationItemManager().setValue(
                                        annotationItem, attribIt->first, attribIt->second);
                            }

                            lattice.addEdge(startVertex, endVertex, annotationItem,
                                    tags, formBuilder.build());
                        }

                    }
                }

            }

            void addNewVariantEdges(Lattice &lattice,
                    Lattice::EdgeDescriptor edge,
                    std::vector<std::string> baseForms,
                    std::vector<Morphology> morphology) {

                Lattice::VertexDescriptor startVertex =
                    lattice.getEdgeBeginIndex(edge);
                Lattice::VertexDescriptor endVertex =
                    startVertex + lattice.getEdgeLength(edge);
                AnnotationItem annotationItem =
                    lattice.getEdgeAnnotationItem(edge);
                LayerTagCollection tags = lattice.getEdgeLayerTags(edge);

                for (std::vector<std::string>::iterator baseIt =
                        baseForms.begin();
                        baseIt != baseForms.end();
                        ++ baseIt) {
                    Lattice::EdgeDescriptor lemmaEdge;
                    if (! matchLemmaEdge(lattice, startVertex, *baseIt)) {
                        LayerTagCollection token_tag
                            = lattice.getLayerTagManager().createSingletonTagCollection("token");
                        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
                        Lattice::EdgeSequence::Builder lemmaBuilder(lattice);
                        lemmaBuilder.addEdge(lattice.firstOutEdge(startVertex, tokenMask));
                        AnnotationItem aiLemma("word", *baseIt);
                        LayerTagCollection tagsLemma = lattice.getLayerTagManager().
                            createSingletonTagCollection("lemma");
                        lemmaEdge = lattice.addEdge(
                                startVertex, endVertex, aiLemma, tagsLemma, lemmaBuilder.build()
                                );
                    } else
                        lemmaEdge = getLemmaEdge(lattice, startVertex, *baseIt);

                    for (std::vector<Morphology>::iterator morphIt =
                            morphology.begin();
                            morphIt != morphology.end();
                            ++ morphIt) {
                        std::string morphoString = util::getMorphologyString(
                                *morphIt);
                        size_t delimPos = morphoString.find(":");
                        std::string partOfSpeech = morphoString.substr(0, delimPos);
                        std::string lexeme = *baseIt + "_" + partOfSpeech;

                        Lattice::EdgeDescriptor lexemeEdge;
                        if (! matchLexemeEdge(lattice, startVertex,
                                    partOfSpeech, lexeme)) {
                            Lattice::EdgeSequence::Builder lexemeBuilder(lattice);
                            lexemeBuilder.addEdge(lemmaEdge);
                            AnnotationItem aiLexeme(partOfSpeech, StringFrag(lexeme));
                            LayerTagCollection tagsLexeme = lattice.getLayerTagManager().
                                createSingletonTagCollection("lexeme");
                            lexemeEdge = lattice.addEdge(
                                    startVertex, endVertex, aiLexeme, tagsLexeme,
                                    lexemeBuilder.build() );
                        } else
                            lexemeEdge = getLexemeEdge(lattice, startVertex,
                                    partOfSpeech, lexeme);

                        Lattice::EdgeSequence::Builder formBuilder(lattice);
                        formBuilder.addEdge(lexemeEdge);
                        AnnotationItem annotationItem(partOfSpeech, StringFrag(lexeme));
                        for (Morphology::iterator attribIt = morphIt->begin();
                                attribIt != morphIt->end(); ++ attribIt) {
                            if (attribIt->first == "pos")
                                continue;
                            lattice.getAnnotationItemManager().setValue(
                                    annotationItem, attribIt->first, attribIt->second);
                        }

                        lattice.addEdge(startVertex, endVertex, annotationItem,
                                tags, formBuilder.build());
                    }
                }
            }

            void removeParseEdges(Lattice &lattice,
                    Lattice::VertexDescriptor start,
                    Lattice::VertexDescriptor end ) {
                LayerTagMask mask = lattice.getLayerTagManager().getMask(
                                lattice.getLayerTagManager().
                                    createSingletonTagCollection("parse")
                        );
                Lattice::VertexDescriptor vertex = start;
                while (vertex <= end) {
                    std::list<Lattice::EdgeDescriptor> edges = getTopEdges(
                            lattice, vertex, mask);
                    if (edges.empty()) {
                        vertex ++;
                        continue;
                    }
                    for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                            edges.begin(); edgeIt != edges.end(); ++ edgeIt) {
                        lattice.discard(*edgeIt);
                    }
                    vertex += lattice.getEdgeLength(edges.front());
                }
            }


            std::string getBase(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                if (isTokenEdge(lattice, edge)) {
                    return lattice.getEdgeText(edge);
                }
                try {
                    Lattice::EdgeDescriptor lemmaEdge =
                        getLemmaEdge(lattice, edge);
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(lemmaEdge);
                    return annotationItem.getText();
                }
                catch (PuddleNoLexemeEdgeException &exception) {
                    return lattice.getEdgeText(edge);
                }
                catch (PuddleNoLemmaEdgeException &exception) {
                    return lattice.getEdgeText(edge);
                }
            }

            std::string getPartOfSpeech(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                if (isTokenEdge(lattice, edge)) {
                    return "ign";
                    // @todo: zrobic to lepiej. interpunkcje moze inaczej moga byc obsluzone zreszta
                }
                try {
                    Lattice::EdgeDescriptor lexemeEdge =
                        getLexemeEdge(lattice, edge);
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(lexemeEdge);
                    return annotationItem.getCategory();
                }
                catch (PuddleNoLexemeEdgeException &exception) {
                    return "ign";
                    // @todo: zrobic to lepiej. interpunkcje moze inaczej moga byc obsluzone zreszta
                }
            }

            bool isParseEdge(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                std::list<std::string> tags =
                    lattice.getLayerTagManager().getTagNames(
                            lattice.getEdgeLayerTags(edge)
                            );
                for (std::list<std::string>::iterator tagIt = tags.begin();
                        tagIt != tags.end(); ++ tagIt) {
                    if (*tagIt == "parse")
                        return true;
                }
                return false;
            }

            Lattice::EdgeDescriptor getLexemeEdge(
                    Lattice &lattice, Lattice::EdgeDescriptor edge) {
                if (isParseEdge(lattice, edge)) {
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(edge);
                    int headEdgeIndex = 0;
                    if (lattice.getAnnotationItemManager().getValue(
                                annotationItem, "head") != "") {
                        headEdgeIndex = boost::lexical_cast<int>(
                                lattice.getAnnotationItemManager().getValue(
                                    annotationItem, "head") );
                    }
                    Lattice::Partition partition =
                        lattice.getEdgePartitions(edge).front();
                    int edgeCount = 0;
                    Lattice::Partition::Iterator edgeIt(lattice, partition);
                    while (edgeIt.hasNext()) {
                        Lattice::EdgeDescriptor ed = edgeIt.next();
                        if (edgeCount == headEdgeIndex) {
                            return getLexemeEdge(lattice, ed);
                        }
                        edgeCount ++;
                    }
                } else {
                    Lattice::EdgeDescriptor lexemeEdge =
                        lattice.getEdgePartitions(edge).front().firstEdge(lattice);
                    std::list<std::string> tags =
                        lattice.getLayerTagManager().getTagNames(
                                lattice.getEdgeLayerTags(lexemeEdge)
                                );
                    bool lexemeTagFound = false;
                    for (std::list<std::string>::iterator tagIt = tags.begin();
                            tagIt != tags.end(); ++ tagIt) {
                        if (*tagIt == "lexeme") {
                            lexemeTagFound = true;
                            break;
                        }
                    }
                    if (lexemeTagFound) {
                        return lexemeEdge;
                    } else {
                        throw PuddleNoLexemeEdgeException("No lexeme edge");
                    }
                }
                throw PuddleNoLexemeEdgeException("No lexeme edge");
            }

            Lattice::EdgeDescriptor getLemmaEdge(
                    Lattice &lattice, Lattice::EdgeDescriptor edge) {
                if (isParseEdge(lattice, edge)) {
                    Lattice::Partition partition =
                        lattice.getEdgePartitions(edge).front();
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(edge);
                    int headEdgeIndex = 0;
                    std::string headString = lattice.getAnnotationItemManager().getValue(
                                annotationItem, "head");
                    if (headString != "") {
                        headEdgeIndex = boost::lexical_cast<int>(
                                lattice.getAnnotationItemManager().getValue(
                                    annotationItem, "head") );
                    }
                    int edgeCount = 0;
                    Lattice::Partition::Iterator edgeIt(lattice, partition);
                    while (edgeIt.hasNext()) {
                        Lattice::EdgeDescriptor ed = edgeIt.next();
                        if (edgeCount == headEdgeIndex) {
                            return getLemmaEdge(lattice, ed);
                        }
                        edgeCount ++;
                    }
                } else {
                    Lattice::EdgeDescriptor lexemeEdge =
                        lattice.getEdgePartitions(edge).front().firstEdge(lattice);
                    std::list<std::string> tags =
                        lattice.getLayerTagManager().getTagNames(
                                lattice.getEdgeLayerTags(lexemeEdge)
                                );
                    bool lexemeTagFound = false;
                    for (std::list<std::string>::iterator tagIt = tags.begin();
                            tagIt != tags.end(); ++ tagIt) {
                        if (*tagIt == "lexeme") {
                            lexemeTagFound = true;
                            break;
                        }
                    }
                    if (lexemeTagFound) {
                        Lattice::EdgeDescriptor lemmaEdge =
                            lattice.getEdgePartitions(lexemeEdge).front().firstEdge(lattice);
                        std::list<std::string> tags =
                            lattice.getLayerTagManager().getTagNames(
                                    lattice.getEdgeLayerTags(lemmaEdge)
                                    );
                        bool lemmaTagFound = false;
                        for (std::list<std::string>::iterator tagIt = tags.begin();
                                tagIt != tags.end(); ++ tagIt) {
                            if (*tagIt == "lemma") {
                                lemmaTagFound = true;
                                break;
                            }
                        }
                        if (lemmaTagFound) {
                            return lemmaEdge;
                        }
                        else {
                            throw PuddleNoLexemeEdgeException("No lemma edge");
                        }
                    } else {
                        throw PuddleNoLexemeEdgeException("No lexeme edge");
                    }
                }
                throw PuddleNoLemmaEdgeException("No lemma edge");
            }

            bool matchLemmaEdge(Lattice &lattice,
                    Lattice::VertexDescriptor vertex,
                    std::string lemma) {
                LayerTagCollection lemmaTag
                    = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
                LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemmaTag);
                return matchEdge(lattice, vertex, lemmaMask, "word", lemma);
            }

            bool matchLexemeEdge(Lattice &lattice,
                    Lattice::VertexDescriptor vertex,
                    std::string partOfSpeech, std::string lexeme) {
                LayerTagCollection lexemeTag
                    = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
                LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexemeTag);
                return matchEdge(lattice, vertex, lexemeMask,
                        partOfSpeech, lexeme);
            }

            bool matchEdge(Lattice &lattice,
                    Lattice::VertexDescriptor vertex,
                    LayerTagMask mask,
                    std::string category, std::string text) {
                Lattice::InOutEdgesIterator edgeIt =
                    lattice.outEdges(vertex, mask);
                while (edgeIt.hasNext()) {
                    Lattice::EdgeDescriptor edge = edgeIt.next();
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(edge);
                    if (annotationItem.getCategory() == category
                            && annotationItem.getText() == text) {
                        return true;
                    }
                }
                return false;
            }

            Lattice::EdgeDescriptor getLemmaEdge(Lattice &lattice,
                    Lattice::VertexDescriptor vertex,
                    std::string lemma) {
                LayerTagCollection lemmaTag
                    = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
                LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemmaTag);
                return getEdge(lattice, vertex, lemmaMask, "word", lemma);
            }

            Lattice::EdgeDescriptor getLexemeEdge(Lattice &lattice,
                    Lattice::VertexDescriptor vertex,
                    std::string partOfSpeech, std::string lexeme) {
                LayerTagCollection lexemeTag
                    = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
                LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexemeTag);
                return getEdge(lattice, vertex, lexemeMask,
                        partOfSpeech, lexeme);
            }

            Lattice::EdgeDescriptor getEdge(Lattice &lattice,
                    Lattice::VertexDescriptor vertex,
                    LayerTagMask mask,
                    std::string category, std::string text) {
                Lattice::InOutEdgesIterator edgeIt =
                    lattice.outEdges(vertex, mask);
                while (edgeIt.hasNext()) {
                    Lattice::EdgeDescriptor edge = edgeIt.next();
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(edge);
                    if (annotationItem.getCategory() == category
                            && annotationItem.getText() == text) {
                        return edge;
                    }
                }
                throw PuddleNoMatchingEdgeException(
                        "No edge of category '" + category + "' with text '"
                        + text + "' found."
                        );
            }

            std::string getMorphologyString(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                std::string partOfSpeech = getPartOfSpeech(lattice, edge);
                std::string morpho = partOfSpeech;
                AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                std::list< std::pair<std::string, std::string> > av
                    = lattice.getAnnotationItemManager().getValues(ai);
                for (std::list< std::pair<std::string, std::string> >::iterator avit =
                        //@todo: sortowanie tego
                        av.begin(); avit != av.end(); ++ avit) {
                    if (avit->first == "head" || avit->first == "orth")
                        continue;
                    morpho += ":";
                    morpho += avit->second;
                }
                return morpho;
            }

            Morphology getMorphology(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                Morphology morphology;
                std::string partOfSpeech = getPartOfSpeech(lattice, edge);
                morphology.insert(std::pair<std::string, std::string>("pos",
                            partOfSpeech));
                AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                std::list< std::pair<std::string, std::string> > av
                    = lattice.getAnnotationItemManager().getValues(ai);
                for (std::list< std::pair<std::string, std::string> >::iterator avit =
                        //@todo: sortowanie tego
                        av.begin(); avit != av.end(); ++ avit) {
//                    if (avit->first == "discard")
//                        continue;
                    if (avit->first == "head" || avit->first == "orth")
                        continue;
                    morphology.insert(std::pair<std::string, std::string>(
                                avit->first, avit->second));
                }
                return morphology;
            }

            bool isDiscarded(Lattice &lattice, Lattice::EdgeDescriptor edge) {
                LayerTagMask mask = lattice.getLayerTagManager().getMask(
                        lattice.getEdgeLayerTags(edge));
                if (lattice.getLayerTagManager().match(mask, "discarded"))
                    return true;
                else
                    return false;
//
//                AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
//                std::list< std::pair<std::string, std::string> > av
//                    = lattice.getAnnotationItemManager().getValues(ai);
//                for (std::list< std::pair<std::string, std::string> >::iterator avit =
//                        av.begin(); avit != av.end(); ++ avit) {
//                    if (avit->first == "discard") {
//                        if (avit->second == "1")
//                            return true;
//                        else
//                            return false;
//                    }
//                }
//                return false;
            }

            bool isTokenEdge(Lattice &lattice, Lattice::EdgeDescriptor edge) {
                LayerTagMask mask = lattice.getLayerTagManager().getMask(
                        lattice.getEdgeLayerTags(edge));
                if (lattice.getLayerTagManager().match(mask, "token"))
                    return true;
                else
                    return false;
            }

            bool isBlankTokenEdge(Lattice &lattice, Lattice::EdgeDescriptor edge) {
                AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);
                if (annotationItem.getCategory() == "B") {
                    return true;
                } else {
                    return false;
                }
            }

            void deleteEdges(Lattice &lattice, Lattice::VertexDescriptor vertex,
                    int count, DeleteConditions conditions) {
                //int offset = currentEntity + before;
                int offset = vertex;
                int vertexI = 0;
                while (vertexI < count) {
                    vertex = getVertex(lattice, vertexI, offset);
                    std::list<Lattice::EdgeDescriptor> edges =
                        lattice::getTopEdges(lattice, vertex);
                    for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt = edges.begin();
                            edgeIt != edges.end(); ++ edgeIt) {
                        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(*edgeIt);
                        if (lattice::isDiscarded(lattice, *edgeIt))
                            continue; //skip discarded edges

                        for (DeleteConditions::iterator cond_it = conditions.begin();
                                cond_it != conditions.end(); ++ cond_it) {
                            if (cond_it->type == BASE_CONDITION) {
                                std::string tokenBase = lattice::getBase(lattice, *edgeIt);
                                if (cond_it->negation) {
                                    if (RegExp::FullMatch(tokenBase, cond_it->pattern)) {
                                        lattice.discard(*edgeIt);
                                    }
                                } else {
                                    if (!RegExp::FullMatch(tokenBase, cond_it->pattern)) {
                                        lattice.discard(*edgeIt);
                                    }
                                }
                            } else if (cond_it->type == MORPHOLOGY_CONDITION) {
                                std::string tokenMorphology = getMorphologyString(
                                        lattice, *edgeIt);
                                if (RegExp::FullMatch(tokenMorphology, cond_it->pattern)) {
                                    lattice.discard(*edgeIt);
                                }
                            }
                        }
                    }
                    vertexI ++;
                }
            }

            bool sequenceContainsEdge(Lattice &lattice,
                    Lattice::EdgeSequence sequence,
                    Lattice::EdgeDescriptor edge) {
                Lattice::VertexDescriptor startVertex =
                    lattice.getEdgeBeginIndex(edge);
                AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);
                Lattice::EdgeSequence::Iterator edgeIt(lattice, sequence);
                while (edgeIt.hasNext()) {
                    Lattice::EdgeDescriptor ed = edgeIt.next();
                    if (lattice.getEdgeBeginIndex(ed) == startVertex) {
                        if (annotationItem ==
                                    lattice.getEdgeAnnotationItem(ed)) {
                            return true;
                        }
                        return false;
                    }
                }
                return false;
            }

            std::list<Lattice::EdgeSequence> getPartitionsContainingEdges(
                    Lattice &lattice,
                    std::list<Lattice::EdgeSequence> &partitions,
                    std::list<Lattice::EdgeDescriptor> edges) {
                if (edges.empty())
                    return partitions;
                std::list<Lattice::EdgeSequence> result;
                std::list<Lattice::EdgeSequence>::iterator seqIt =
                        partitions.begin();
                while (seqIt != partitions.end()) {
                    bool containsAllEdges = true;
                    for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                            edges.begin();
                            edgeIt != edges.end();
                            ++ edgeIt) {
                        if (! sequenceContainsEdge(lattice, *seqIt, *edgeIt)) {
                            containsAllEdges = false;
                            break;
                        }
                    }
                    if (containsAllEdges) {
                        result.push_back(*seqIt);
                    }
                    ++ seqIt;
                }
                return result;
            }

            }

        }
    }
}


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
                        std::string orth = lattice.getEdgeText(edge); //@todo: przerabiac to na wygrzebywaine tej formy z krawedzi 'token' czy zostawic jak jest?
                        AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                        std::string category = ai.getCategory();
                        std::list< std::pair<std::string, std::string> > av
                            = lattice.getAnnotationItemManager().getValues(ai);
                        //if (lattice.getAnnotationItemManager().getValue(
                        //           ai, "discard") == "1")
                        //    continue; //skip discarded edges
                        std::string base = getBase(lattice, edge);
                        std::string partOfSpeech = getPartOfSpeech(lattice, edge);
                        std::string morphology = getMorphologyString(lattice, edge);
                        //std::string morphology = partOfSpeech;
                        //for (std::list< std::pair<std::string, std::string> >::iterator avit =
                        //        av.begin(); avit != av.end(); avit ++) {
                        //    morphology += ":" + avit->second;
                        //    std::cerr << "dodaje: " << avit->second << std::endl;
                        //}

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
                        std::string orth = lattice.getEdgeText(edge); //@todo: przerabiac to na wygrzebywaine tej formy z krawedzi 'token' czy zostawic jak jest?
                        AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                        std::string category = ai.getCategory();
                        std::list< std::pair<std::string, std::string> > av
                            = lattice.getAnnotationItemManager().getValues(ai);
                        //if (lattice.getAnnotationItemManager().getValue(
                        //            ai, "discard") == "1")
                        if (isDiscarded(lattice, edge))
                            continue; //skip discarded edges
                        std::string base = getBase(lattice, edge);
                        std::string partOfSpeech = getPartOfSpeech(lattice, edge);
                        std::string morphology = lattice::getMorphologyString(
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
                                createSingletonTagCollection("form")
                                ,
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
                        //if (lattice.getAnnotationItemManager().getValue(
                        //            ai, "discard") == "1")
                        if (isDiscarded(lattice, edge))
                            continue; //skip discarded edges
                        std::string base = lattice::getBase(lattice, edge);
                        std::string partOfSpeech = lattice::getPartOfSpeech(lattice, edge);
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
                                        if (edgesMapIt.first->second->getDepth() > max_depth) //@todo: po co tu szukanie tego max_depth?
                                            max_depth = edgesMapIt.first->second->getDepth();

                                        if (edgesMapIt.first->second->getLabel() == category) { //@todo: do czego ta morfologia jest naprawde doczepiana?
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
                                        edgesMap.insert(std::pair<std::pair<int, int>, TransitionInfo*>(
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
                    int edgeIndex) {
                LayerTagMask mask = lattice.getLayerTagManager().getMask(
                        createUnion(
                            lattice.getLayerTagManager().
                            createSingletonTagCollection("form")
                            ,
                            lattice.getLayerTagManager().
                            createSingletonTagCollection("parse")
                            )
                        );
                return getVertex(lattice, edgeIndex, mask);
            }

            Lattice::VertexDescriptor getVertex(Lattice &lattice,
                    int edgeIndex, LayerTagMask mask) {
                Lattice::VertexDescriptor vertex = 0;
                int edgePosition = 0;
                while (edgePosition < edgeIndex) {
                    Lattice::InOutEdgesIterator edgeIt =
                        lattice.outEdges(vertex, mask);
                    if (! edgeIt.hasNext()) {
                        edgePosition ++;
                        vertex ++;
                        continue;
                    }

                    Lattice::EdgeDescriptor edge;
                    while (edgeIt.hasNext())
                        edge = edgeIt.next();
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
                return vertex;
            }

            std::list<Lattice::EdgeDescriptor> getTopEdges(
                    Lattice &lattice, Lattice::VertexDescriptor start) {
                LayerTagMask mask = lattice.getLayerTagManager().getMask(
                        createUnion(
                            lattice.getLayerTagManager().
                                createSingletonTagCollection("form")
                                ,
                                lattice.getLayerTagManager().
                                    createSingletonTagCollection("parse")
                            )
                        );
                return getTopEdges(lattice, start, mask);
            }

            std::list<Lattice::EdgeDescriptor> getTopEdges(
                    Lattice &lattice, Lattice::VertexDescriptor start,
                    LayerTagMask mask) {
                std::list<Lattice::EdgeDescriptor> edges;
                std::list<Lattice::EdgeDescriptor> nontopEdges;
                Lattice::InOutEdgesIterator edgeIt =
                    lattice.outEdges(start, mask);
                while (edgeIt.hasNext()) {
                    Lattice::EdgeDescriptor edge = edgeIt.next();
                    AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                    //if (lattice.getAnnotationItemManager().getValue(
                    //            ai, "discard") == "1")
                    if (isDiscarded(lattice, edge))
                        continue;

                    edges.push_back(edge);
                    std::list<Lattice::Partition> partitions =
                        lattice.getEdgePartitions(edge);
                    for (std::list<Lattice::Partition>::iterator partIt =
                            partitions.begin();
                            partIt != partitions.end(); ++ partIt) {
                        nontopEdges.insert(nontopEdges.end(),
                                partIt->begin(),
                                partIt->end()
                                );
                    }
                }
                nontopEdges.unique(EdgeUnique());
                edges.remove_if(EdgeNonTop(nontopEdges));
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
                                Lattice::EdgeSequence::Builder rangeBuilder;
                                    for (Lattice::EdgeSequence::Iterator rangeIt =
                                            seqIt->begin(); rangeIt != seqIt->end();
                                            ++ rangeIt)
                                        rangeBuilder.addEdge(*rangeIt);
                                rangeBuilder.addEdge(*edgeIt);
                                tmpRanges.push_back(rangeBuilder.build());
                            }
                        }
                    } else {
                        for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                                edges.begin(); edgeIt != edges.end(); ++ edgeIt) {
                            Lattice::EdgeSequence::Builder rangeBuilder;
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
                LayerTagMask mask = lattice.getLayerTagManager().getMask(
                        createUnion(
                            lattice.getLayerTagManager().
                            createSingletonTagCollection("form")
                            ,
                            lattice.getLayerTagManager().
                            createSingletonTagCollection("parse")
                            )
                        );
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
                        bool firstPartition = true;
                        Lattice::InOutEdgesIterator outEdgesIt = lattice.outEdges(
                                startVertex, lattice.getLayerTagManager().getMask(tags));
                        while (outEdgesIt.hasNext()) {
                            Lattice::EdgeDescriptor newEdge = outEdgesIt.next();
                            Lattice::VertexDescriptor readEnd = startVertex +
                                lattice.getEdgeLength(newEdge);
                            if (readEnd != endVertex)
                                continue;
                            if (areAnnotationItemsEqual(lattice, annotationItem,
                                        lattice.getEdgeAnnotationItem(newEdge))) {
                                //there is already such an edge. add another partition
                                //@todo: to tak nie dziala. nowa partition powinno dodawac samo addEdge, ale nie robi tego. na razie jest wiec jedna partition zawsze

                                firstPartition = false;
                                break;
                            }
                        }
                        if (firstPartition) {
                            //there is no such an edge yet. add a new one
                            lattice.addEdge(
                                    startVertex,
                                    endVertex,
                                    annotationItem,
                                    tags,
                                    *seqIt
                                    );
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
                        Lattice::EdgeSequence::Builder lemmaBuilder;
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

                        //std::string morphology = *morphIt;
                        std::string morphoString = util::getMorphologyString(
                                *morphIt);
                        size_t delimPos = morphoString.find(":");
                        std::string partOfSpeech = morphoString.substr(0, delimPos);
                        std::string morpho = "";
                        if ( (delimPos != std::string::npos) &&
                                (delimPos < morphoString.size()) )
                            morpho = morphoString.substr(delimPos + 1, std::string::npos);
                        std::string lexeme = *baseIt + "_" + partOfSpeech;

                        Lattice::EdgeDescriptor lexemeEdge;
                        if (! matchLexemeEdge(lattice, startVertex,
                                    partOfSpeech, lexeme)) {
                            Lattice::EdgeSequence::Builder lexemeBuilder;
                            lexemeBuilder.addEdge(lemmaEdge);
                            AnnotationItem aiLexeme(partOfSpeech, lexeme);
                            LayerTagCollection tagsLexeme = lattice.getLayerTagManager().
                                createSingletonTagCollection("lexeme");
                            lexemeEdge = lattice.addEdge(
                                    startVertex, endVertex, aiLexeme, tagsLexeme, lexemeBuilder.build()
                                    );
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
                                    annotationItem, "discard", "0");
                            lattice.getAnnotationItemManager().setValue(
                                    annotationItem, "orth", concatenatedOrth);
                            lattice.getAnnotationItemManager().setValue(
                                    annotationItem, "head", "0"); //@todo: czy tego tu inaczej jakos nie trzeba zrobic

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
                            Lattice::EdgeSequence::Builder formBuilder;
                            formBuilder.addEdge(lexemeEdge);
                            AnnotationItem annotationItem(partOfSpeech, lexeme);
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
                                    annotationItem, "discard", "0");

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
                        Lattice::EdgeSequence::Builder lemmaBuilder;
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
                        std::string morpho = "";
                        size_t delimPos = morphoString.find(":");
                        std::string partOfSpeech = morphoString.substr(0, delimPos);
                        if (delimPos != std::string::npos)
                            morpho = morphoString.substr(delimPos + 1, std::string::npos);
                        std::string lexeme = *baseIt + "_" + partOfSpeech;

                        Lattice::EdgeDescriptor lexemeEdge;
                        if (! matchLexemeEdge(lattice, startVertex,
                                    partOfSpeech, lexeme)) {
                            Lattice::EdgeSequence::Builder lexemeBuilder;
                            lexemeBuilder.addEdge(lemmaEdge);
                            AnnotationItem aiLexeme(partOfSpeech, lexeme);
                            LayerTagCollection tagsLexeme = lattice.getLayerTagManager().
                                createSingletonTagCollection("lexeme");
                            lexemeEdge = lattice.addEdge(
                                    startVertex, endVertex, aiLexeme, tagsLexeme, lexemeBuilder.build()
                                    );
                        } else
                            lexemeEdge = getLexemeEdge(lattice, startVertex,
                                    partOfSpeech, lexeme);

                        Lattice::EdgeSequence::Builder formBuilder;
                        formBuilder.addEdge(lexemeEdge);
                        AnnotationItem annotationItem(partOfSpeech, lexeme);
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
                                annotationItem, "discard", "0");

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
                        AnnotationItem ai = lattice.getEdgeAnnotationItem(*edgeIt);
                        lattice.getAnnotationItemManager().setValue(
                                ai, "discard", "1");
                    }
                    vertex += lattice.getEdgeLength(edges.front());
                }
            }

            bool areAnnotationItemsEqual(Lattice &lattice,
                    AnnotationItem a,
                    AnnotationItem b) {
                if (a.getCategory() != b.getCategory())
                    return false;
                std::list<std::pair<std::string, std::string> > valuesA =
                    lattice.getAnnotationItemManager().getValues(a);
                std::list<std::pair<std::string, std::string> > valuesB =
                    lattice.getAnnotationItemManager().getValues(b);
                if (valuesA.size() != valuesB.size())
                    return false;
                std::list< std::pair<std::string, std::string> >::iterator avi =
                    valuesA.begin();
                std::list< std::pair<std::string, std::string> >::iterator bvi =
                    valuesB.begin();
                while (avi != valuesA.end()) {
                    if (avi->first != bvi->first)
                        return false;
                    if (avi->second != bvi->second)
                        return false;
                    ++ avi;
                    ++ bvi;
                }
                return true;
            }


            std::string getBase(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                Lattice::EdgeDescriptor lemmaEdge =
                    getLemmaEdge(lattice, edge);
                AnnotationItem annotationItem =
                    lattice.getEdgeAnnotationItem(lemmaEdge);
                return annotationItem.getText();
            }

            std::string getPartOfSpeech(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                Lattice::EdgeDescriptor lexemeEdge =
                    getLexemeEdge(lattice, edge);
                AnnotationItem annotationItem =
                    lattice.getEdgeAnnotationItem(lexemeEdge);
                return annotationItem.getCategory();
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
                    for (Lattice::Partition::Iterator edgeIt = partition.begin();
                            edgeIt != partition.end(); ++ edgeIt) {
                        if (edgeCount == headEdgeIndex) {
                            return getLexemeEdge(lattice, *edgeIt);
                        }
                        edgeCount ++;
                    }
                } else {
                    Lattice::EdgeDescriptor lexemeEdge =
                        lattice.getEdgePartitions(edge).front().firstEdge();
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
                        //@todo: raczej rzucic wyjatek
                        Lattice::EdgeDescriptor emptyEdge;
                        return emptyEdge;
                    }
                }
                Lattice::EdgeDescriptor emptyEdge;
                return emptyEdge;
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
                    for (Lattice::Partition::Iterator edgeIt = partition.begin();
                            edgeIt != partition.end(); ++ edgeIt) {
                        if (edgeCount == headEdgeIndex) {
                            return getLemmaEdge(lattice, *edgeIt);
                        }
                        edgeCount ++;
                    }
                } else {
                    Lattice::EdgeDescriptor lexemeEdge =
                        lattice.getEdgePartitions(edge).front().firstEdge();
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
                            lattice.getEdgePartitions(lexemeEdge).front().firstEdge();
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
                        if (lemmaTagFound)
                            return lemmaEdge;
                        else {
                            //@todo: raczej rzucic wyjatek
                            Lattice::EdgeDescriptor emptyEdge;
                            return emptyEdge;
                        }
                    } else {
                        //@todo: raczej rzucic wyjatek
                        Lattice::EdgeDescriptor emptyEdge;
                        return emptyEdge;
                    }
                }
                Lattice::EdgeDescriptor emptyEdge;
                return emptyEdge;
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
                Lattice::EdgeDescriptor edge; //@todo: wyjatek
                return edge;
            }

            std::string getMorphologyString(Lattice &lattice,
                    Lattice::EdgeDescriptor edge) {
                std::string partOfSpeech = getPartOfSpeech(lattice, edge);
                std::string morpho = partOfSpeech;
                AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                std::list< std::pair<std::string, std::string> > av
                    = lattice.getAnnotationItemManager().getValues(ai);
                for (std::list< std::pair<std::string, std::string> >::iterator avit = //@todo: sortowanie tego
                        av.begin(); avit != av.end(); ++ avit) {
                    if (avit->first == "discard")
                        continue;
                    morpho += ":";
                    morpho += avit->second;
                }
                return morpho;
            }

            bool isDiscarded(Lattice &lattice, Lattice::EdgeDescriptor edge) {
                AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                std::list< std::pair<std::string, std::string> > av
                    = lattice.getAnnotationItemManager().getValues(ai);
                for (std::list< std::pair<std::string, std::string> >::iterator avit =
                        av.begin(); avit != av.end(); ++ avit) {
                    if (avit->first == "discard") {
                        if (avit->second == "1")
                            return true;
                        else
                            return false;
                    }
                }
                return false;
            }

            }

        }
    }
}

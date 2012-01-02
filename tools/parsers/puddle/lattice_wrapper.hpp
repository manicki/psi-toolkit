#ifndef PUDDLE_LATTICE_WRAPPER_HDR
#define PUDDLE_LATTICE_WRAPPER_HDR

#include "config.hpp"

#include "lattice.hpp"
#include "tagset.hpp"
#include "puddle_util.hpp"
#include "puddle_exceptions.hpp"
#if _WITH_BONSAI_PARSEGRAPH
#include "ParseGraph.hpp"
#endif
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace poleng {
    namespace bonsai {
        namespace puddle {

            namespace lattice {


            std::string readInputLattice(Lattice &lattice);
            std::string readMorfologikLattice(Lattice &lattice);
#if _WITH_BONSAI_PARSEGRAPH
                    ParseGraphPtr convertToBonsaiGraph(Lattice &lattice);
#endif
                    Lattice::VertexDescriptor getVertex(Lattice &lattice,
                            //@todo: zmienic nazwe na getStartVertex
                            int edgeIndex, LayerTagMask mask, int offset = 0);
                    Lattice::VertexDescriptor getVertex(Lattice &lattice,
                            //@todo: zmienic nazwe na getStartVertex
                            int edgeIndex, int offset = 0);
                    std::list<Lattice::EdgeDescriptor> getTopEdges(Lattice &lattice,
                            Lattice::VertexDescriptor start, LayerTagMask mask);
                    std::list<Lattice::EdgeDescriptor> getTopEdges(Lattice &lattice,
                            Lattice::VertexDescriptor start);
                    std::list<Lattice::EdgeSequence> getEdgesRange(Lattice &lattice,
                            Lattice::VertexDescriptor start,
                            Lattice::VertexDescriptor end,
                            LayerTagMask mask);
                    std::list<Lattice::EdgeSequence> getEdgesRange(Lattice &lattice,
                            Lattice::VertexDescriptor start,
                            Lattice::VertexDescriptor end);
                    void addParseEdges(Lattice &lattice,
                            std::list<Lattice::EdgeDescriptor> startEdges,
                            std::list<Lattice::EdgeDescriptor> endEdges,
                            std::string &parseCategory,
                            std::list<Lattice::EdgeDescriptor> headEdges,
                            std::list<Lattice::EdgeSequence> groupSequences,
                            int headEdgeIndex,
                            Lattice::Score score = 0.0);
                    void addSyntokEdges(Lattice &lattice,
                            std::list<Lattice::EdgeDescriptor> startEdges,
                            std::list<Lattice::EdgeDescriptor> endEdges,
                            std::string &syntokCategory,
                            std::string &concatenatedOrth,
                            std::vector<std::string> baseForms,
                            std::vector<Morphology> morphology,
                            std::list<Lattice::EdgeSequence> edgeSequences,
                            LayerTagCollection tags,
                            Lattice::Score score = 0.0);
                    void addNewVariantEdges(Lattice &lattice,
                            Lattice::EdgeDescriptor edge,
                            std::vector<std::string> baseForms,
                            std::vector<Morphology> morphology);
                    void removeParseEdges(Lattice &lattice,
                            Lattice::VertexDescriptor start,
                            Lattice::VertexDescriptor end);
                    void deleteEdges(Lattice &lattice,
                            Lattice::VertexDescriptor vertex, int count,
                            DeleteConditions conditions);
                    std::string getBase(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
                    std::string getPartOfSpeech(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
                    bool isParseEdge(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
                    Lattice::EdgeDescriptor getLexemeEdge(
                            Lattice &lattice, Lattice::EdgeDescriptor edge);
                    Lattice::EdgeDescriptor getLemmaEdge(
                            Lattice &lattice, Lattice::EdgeDescriptor edge);
                    bool matchLemmaEdge(Lattice &lattice,
                            Lattice::VertexDescriptor vertex,
                            std::string lemma);
                    bool matchLexemeEdge(Lattice &lattice,
                            Lattice::VertexDescriptor vertex,
                            std::string partOfSpeech, std::string lexeme);
                    bool matchEdge(Lattice &lattice,
                            Lattice::VertexDescriptor vertex,
                            LayerTagMask mask,
                            std::string category, std::string text);
                    Lattice::EdgeDescriptor getLemmaEdge(Lattice &lattice,
                            Lattice::VertexDescriptor vertex,
                            std::string lemma);
                    Lattice::EdgeDescriptor getLexemeEdge(Lattice &lattice,
                            Lattice::VertexDescriptor vertex,
                            std::string partOfSpeech, std::string lexeme);
                    Lattice::EdgeDescriptor getEdge(Lattice &lattice,
                            Lattice::VertexDescriptor vertex,
                            LayerTagMask mask,
                            std::string category, std::string text);
                    bool isDiscarded(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
                    bool isTokenEdge(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
                    bool isBlankTokenEdge(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
                    bool sequenceContainsEdge(Lattice &lattice,
                            Lattice::EdgeSequence sequence,
                            Lattice::EdgeDescriptor edge);
                    std::list<Lattice::EdgeSequence> getPartitionsContainingEdges(
                            Lattice &lattice,
                            std::list<Lattice::EdgeSequence> &partitions,
                            std::list<Lattice::EdgeDescriptor> edges);

//                    bool areAnnotationItemsEqual(Lattice &lattice,
//                            AnnotationItem a,
//                            AnnotationItem b);

                    std::string getMorphologyString(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
                    Morphology getMorphology(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);

            class EdgeNonTop {
                private:
                    Lattice &lattice;
                    std::list<Lattice::EdgeDescriptor> nontopEdges;
                    int maxStart, maxEnd;
                public:
                    EdgeNonTop(Lattice &aLattice,
                            std::list<Lattice::EdgeDescriptor> aNontopEdges)
                        : lattice(aLattice), nontopEdges(aNontopEdges) {
                            maxStart = lattice.getLastVertex();
                            maxEnd = 0;
                            for (std::list<Lattice::EdgeDescriptor>::iterator it =
                                    nontopEdges.begin();
                                    it != nontopEdges.end();
                                    ++ it) {
                                int start = lattice.getEdgeBeginIndex(*it);
                                int length = lattice.getEdgeLength(*it);
                                if (start < maxStart)
                                    maxStart = start;
                                if ( (start + length) > maxEnd)
                                    maxEnd = start + length;
                            }
                        }
                    bool operator() (const Lattice::EdgeDescriptor &value) {
                        Lattice::VertexDescriptor start =
                            lattice.getEdgeBeginIndex(value);
                        Lattice::VertexDescriptor length =
                            lattice.getEdgeLength(value);
                        if (start > maxStart)
                            return true;
                        if ( (start + length) < maxEnd)
                            return true;
                        AnnotationItem annotationItem =
                            lattice.getEdgeAnnotationItem(value);
                        for (std::list<Lattice::EdgeDescriptor>::iterator it =
                                nontopEdges.begin(); it != nontopEdges.end();
                                ++ it) {
                            if (start == lattice.getEdgeBeginIndex(*it)) {
                                if (length == lattice.getEdgeLength(*it)) {
                                    //if (areAnnotationItemsEqual(lattice, annotationItem,
                                    //            lattice.getEdgeAnnotationItem(*it))) {
                                    if (annotationItem ==
                                                lattice.getEdgeAnnotationItem(*it)) {
                                        return true;
                                    }
                                }
                            }
                        }
                                return false;
//                        for (std::list<Lattice::EdgeDescriptor>::iterator it =
//                                nontopEdges.begin(); it != nontopEdges.end();
//                                ++ it) {
//                            //if (*it == value)
//                            if (! ((*it < value) || (value < *it)) )
//                                return true;
//                        }
//                        return false;
                    }
            };

            class EdgeUnique {
                public:
                    bool operator() (const Lattice::EdgeDescriptor &first,
                            const Lattice::EdgeDescriptor &second) const {
                        return (! ((first < second) || (second < first)) );
                    }
            };

            class EdgeNonToken {
                private:
                    Lattice &lattice;
                public:
                    EdgeNonToken(Lattice &aLattice)
                        : lattice(aLattice) { }
                    bool operator() (const Lattice::EdgeDescriptor &value) {
                        if (isTokenEdge(lattice, value))
                            return true;
                        else
                            return false;
                    }
            };


            }
        }
    }
}

#endif

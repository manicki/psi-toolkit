#ifndef PUDDLE_LATTICE_WRAPPER_H__
#define PUDDLE_LATTICE_WRAPPER_H__

#include "config.h"

#include "lattice.hpp"
#include "tagset.hpp"
#include "puddle_util.hpp"
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

            class EdgeNonTop {
                private:
                    std::list<Lattice::EdgeDescriptor> nontopEdges;
                public:
                    EdgeNonTop(std::list<Lattice::EdgeDescriptor> aNontopEdges)
                        : nontopEdges(aNontopEdges) {}
                    bool operator() (const Lattice::EdgeDescriptor &value) {
                        for (std::list<Lattice::EdgeDescriptor>::iterator it =
                                nontopEdges.begin(); it != nontopEdges.end();
                                ++ it) {
                            //if (*it == value)
                            if (! ((*it < value) || (value < *it)) )
                                return true;
                        }
                        return false;
                    }
            };

            class EdgeUnique {
                public:
                    bool operator() (const Lattice::EdgeDescriptor &first,
                            const Lattice::EdgeDescriptor &second) const {
                        return (! ((first < second) || (second < first)) );
                    }
            };


            std::string readInputLattice(Lattice &lattice);
            std::string readMorfologikLattice(Lattice &lattice);
#if _WITH_BONSAI_PARSEGRAPH
                    ParseGraphPtr convertToBonsaiGraph(Lattice &lattice);
#endif
                    Lattice::VertexDescriptor getVertex(Lattice &lattice,
                            int edgeIndex, LayerTagMask mask);
                    Lattice::VertexDescriptor getVertex(Lattice &lattice,
                            int edgeIndex);
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

                    bool areAnnotationItemsEqual(Lattice &lattice,
                            AnnotationItem a,
                            AnnotationItem b);

                    std::string getMorphologyString(Lattice &lattice,
                            Lattice::EdgeDescriptor edge);
            }
        }
    }
}

#endif

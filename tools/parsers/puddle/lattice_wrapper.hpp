#ifndef PUDDLE_LATTICE_WRAPPER_H__
#define PUDDLE_LATTICE_WRAPPER_H__

#include "lattice.hpp"
#include "tagset.hpp"
#ifdef _WITH_BONSAI_PARSEGRAPH
#include "ParseGraph.hpp"
#endif
#include <iostream>
#include <sstream>

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
                            const Lattice::EdgeDescriptor &second) {
                        return (! ((first < second) || (second < first)) );
                    }
            };


            //class LatticeWrapper {
            //    public:
            //        LatticeWrapper(); //TagsetPtr tagset);
            std::string readInputLattice(Lattice &lattice);
#ifdef _WITH_BONSAI_PARSEGRAPH
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
                            Lattice::Score score = 0.0);
                    void addSyntokEdges(Lattice &lattice,
                            std::list<Lattice::EdgeDescriptor> startEdges,
                            std::list<Lattice::EdgeDescriptor> endEdges,
                            std::string &syntokCategory,
                            std::string &concatenatedOrth,
                            std::vector<std::string> baseForms,
                            std::vector<std::string> morphology,
                            std::list<Lattice::EdgeSequence> edgeSequences,
                            LayerTagCollection tags,
                            Lattice::Score score = 0.0);
                    void addNewVariantEdges(Lattice &lattice,
                            Lattice::EdgeDescriptor edge,
                            std::vector<std::string> baseForms,
                            std::vector<std::string> morphology);
                    void removeParseEdges(Lattice &lattice,
                            Lattice::VertexDescriptor start,
                            Lattice::VertexDescriptor end);
//                    void addPosEdges(Lattice &lattice);
//                private:
                    //AnnotationItemManager annotationItemManager;
                    //void initAnnotationItemManager(TagsetPtr tagset);
//                    bool isNonTop(std::list<Lattice::EdgeDescriptor> nontopEdges,
//                            const Lattice::EdgeDescriptor &value);
            //};
                    bool areAnnotationItemsEqual(Lattice &lattice,
                            AnnotationItem a,
                            AnnotationItem b);


            //typedef boost::shared_ptr<LatticeWrapper> LatticeWrapperPtr;
            }
        }
    }
}

#endif

#ifndef PUDDLE_LATTICE_WRAPPER_H__
#define PUDDLE_LATTICE_WRAPPER_H__

#include "lattice.hpp"
#include "ParseGraph.hpp"
#include "tagset.hpp"

namespace poleng {
    namespace bonsai {
        namespace puddle {

            class LatticeWrapper {
                public:
                    LatticeWrapper(TagsetPtr tagset);
                    ParseGraphPtr readInputLattice(Lattice &lattice,
                            std::string &sentenceString);
                    /*std::list<Lattice::EdgeDescriptor> getEdge(Lattice &lattice,
                            Lattice::VertexDescriptor start, LayerTagMask mask);
                    void addParseEdge(Lattice &lattice, Lattice::VertexDescriptor start,
                            Lattice::VertexDescriptor end, std::string &parseLabel);*/
                private:
                    AnnotationItemManager annotationItemManager;
                    void initAnnotationItemManager(TagsetPtr tagset);
            };
        }
    }
}

#endif

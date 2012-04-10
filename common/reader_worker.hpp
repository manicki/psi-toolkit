#ifndef READER_WORKER_HDR
#define READER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

template<typename Source>
class ReaderWorker : public LatticeWorker {

public:
    ReaderWorker(Source& source, Lattice& lattice)
        :LatticeWorker(lattice), inputStream_(source) {
    }

    virtual ~ReaderWorker() {
    }

protected:
    std::istream& inputStream_;

    void appendParagraphToLattice_(std::string paragraph, LayerTagCollection tags) {
        if (!paragraph.empty()) {
            Lattice::VertexDescriptor prevEnd = lattice_.getLastVertex();
            lattice_.appendStringWithSymbols(paragraph);
            Lattice::VertexDescriptor nowEnd = lattice_.getLastVertex();
            AnnotationItem item("FRAG", paragraph);
            lattice_.addEdge(prevEnd, nowEnd, item, tags);
        }
    }

};

#endif

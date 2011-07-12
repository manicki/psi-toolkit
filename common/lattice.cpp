#include "lattice.hpp"

Lattice::Lattice(std::string text) {
    //TODO
}

Lattice::~Lattice() {
    //TODO
}

Lattice::VertexDescriptor Lattice::getVertexForRawCharIndex(int ix) {
    //TODO
}

Lattice::VertexDescriptor Lattice::getFirstVertex() {
    //TODO
}

Lattice::VertexDescriptor Lattice::getLastVertex() {
    //TODO
}

Lattice::EdgeDescriptor Lattice::addEdge(
    VertexDescriptor from,
    VertexDescriptor to,
    const AnnotationItem& annotation_item,
    LayerTagCollection tags,
    std::list<EdgeDescriptor> partition
) {
    //TODO
}

std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator> Lattice::outEdges(Lattice::VertexDescriptor vertex, LayerTagCollection mask) const {
    //TODO
}

std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator> Lattice::inEdges(Lattice::VertexDescriptor vertex, LayerTagCollection mask) const {
    //TODO
}

Lattice::EdgeDescriptor Lattice::firstOutEdge(Lattice::VertexDescriptor vertex, LayerTagCollection mask) const {
    //TODO
}

Lattice::EdgeDescriptor Lattice::firstInEdge(Lattice::VertexDescriptor vertex, LayerTagCollection mask) const {
    //TODO
}

std::list<Lattice::EdgeDescriptor> Lattice::edgesSortedTopologically(LayerTagCollection mask) {
    //TODO
}

LayerTagManager& Lattice::getLayerTagManager() {
    //TODO
}

const AnnotationItem& Lattice::getEdgeAnnotationItem(Lattice::EdgeDescriptor edge) {
    //TODO
}

const LayerTagCollection& Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) {
    //TODO
}


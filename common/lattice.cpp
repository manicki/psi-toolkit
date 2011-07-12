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
    const AnnotationItem& annotationItem,
    LayerTagCollection tags,
    std::list<EdgeDescriptor> partition
) {
    std::pair<EdgeDescriptor, bool> result = boost::add_edge(from, to, EdgeEntry(annotationItem, tags), g_);
    return result.first;
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


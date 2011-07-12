#include "lattice.hpp"

Lattice::Lattice(std::string text) : layerTagManager_() {
    for (int i = 0; i < text.length(); ++i) {
        vertices_.push_back(boost::add_vertex(graph_));
    }
}

Lattice::~Lattice() {
    //TODO
}

Lattice::VertexDescriptor Lattice::getVertexForRawCharIndex(int ix) {
    return vertices_[ix];
}

Lattice::VertexDescriptor Lattice::getFirstVertex() {
    return vertices_.front();
}

Lattice::VertexDescriptor Lattice::getLastVertex() {
    return vertices_.back();
}

Lattice::EdgeDescriptor Lattice::addEdge(
    VertexDescriptor from,
    VertexDescriptor to,
    const AnnotationItem& annotationItem,
    LayerTagCollection tags,
    std::list<EdgeDescriptor> partition
) {
    std::pair<EdgeDescriptor, bool> result = boost::add_edge(from, to, EdgeEntry(annotationItem, tags), graph_);
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
    return layerTagManager_;
}

const AnnotationItem& Lattice::getEdgeAnnotationItem(Lattice::EdgeDescriptor edge) {
    //TODO
}

const LayerTagCollection& Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) {
    //TODO
}


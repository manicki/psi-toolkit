#include "lattice.hpp"

Lattice::Lattice(std::string text) : layerTagManager_() {
    for (int i = 0; i < text.length(); ++i) {
        Lattice::VertexDescriptor vertex = boost::add_vertex(graph_);
        if (!vertices_.empty()) {
            boost::add_edge(
                vertices_.back(), 
                vertex, 
                EdgeEntry(
                    AnnotationItem(text[i]), 
                    LayerTagManager.createSingletonTagCollection("raw")
                ), 
                graph_
            );
        }
        vertices_.push_back(vertex);
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
    std::list<EdgeDescriptor> edgeList;
    std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator> allOutEdges = boost::out_edges(vertex, graph_);
    for (
        Lattice::OutEdgeIterator oei = allOutEdges.first;
        oei != allOutEdges.second;
        ++oei
    ) {
        if (!createIntersection(graph_[*oei].tagList, mask).isEmpty()) {
            edgeList.push_back(*oei);
        }
    }
    return std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator>(
        (Lattice::OutEdgeIterator)edgeList.begin(), 
        (Lattice::OutEdgeIterator)edgeList.end()
    );
}

std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator> Lattice::inEdges(Lattice::VertexDescriptor vertex, LayerTagCollection mask) const {
    std::list<EdgeDescriptor> edgeList;
    std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator> allInEdges = boost::in_edges(vertex, graph_);
    for (
        Lattice::InEdgeIterator iei = allInEdges.first;
        iei != allInEdges.second;
        ++iei
    ) {
        if (!createIntersection(graph_[*iei].tagList, mask).isEmpty()) {
            edgeList.push_back(*iei);
        }
    }
    return std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator>(edgeList.begin(), edgeList.end());
}

Lattice::EdgeDescriptor Lattice::firstOutEdge(Lattice::VertexDescriptor vertex, LayerTagCollection mask) const {
    std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator> allOutEdges = boost::out_edges(vertex, graph_);
    for (
        Lattice::OutEdgeIterator oei = allOutEdges.begin();
        oei != allOutEdges.end();
        ++oei
    ) {
        if (!createIntersection(graph_[*oei].tagList, mask).isEmpty()) {
            return *oei;
        }
    }
    return NULL;
}

Lattice::EdgeDescriptor Lattice::firstInEdge(Lattice::VertexDescriptor vertex, LayerTagCollection mask) const {
    std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator> allInEdges = boost::in_edges(vertex, graph_);
    for (
        Lattice::InEdgeIterator iei = allInEdges.begin();
        iei != allInEdges.end();
        ++iei
    ) {
        if (!createIntersection(graph_[*iei].tagList, mask).isEmpty()) {
            return *iei;
        }
    }
    return NULL;
}

std::list<Lattice::EdgeDescriptor> Lattice::edgesSortedTopologically(LayerTagCollection mask) {
    std::list<Lattice::EdgeDescriptor> result;
    for (
        std::vector<Lattice::VertexDescriptor>::iterator vi = vertices_.begin();
        vi != vertices_.end();
        ++vi
    ) {
        std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator> outEdgesIters = outEdges(*vi);
        result.insert(result.end(), outEdgesIters.first, outEdgesIters.second);
    }
    return result;
}

LayerTagManager& Lattice::getLayerTagManager() {
    return layerTagManager_;
}

const AnnotationItem& Lattice::getEdgeAnnotationItem(Lattice::EdgeDescriptor edge) {
    return graph_[edge].category;
}

const LayerTagCollection& Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) {
    return graph_[edge].tagList;
}


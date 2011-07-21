#include "lattice.hpp"

Lattice::Lattice(std::string text) : layerTagManager_(), implicitOutEdges_(text.length() + 1) {
    Graph::vertex_descriptor vertex = boost::add_vertex(graph_);
    vertices_[0] = vertex;
    for (int j = 0; j < indexedTagCollections_.size(); ++j) {
        graph_[vertex].outEdgesIndex.push_back(std::list<EdgeDescriptor>());
        graph_[vertex].inEdgesIndex.push_back(std::list<EdgeDescriptor>());
    }
    appendString(text);
}

Lattice::~Lattice() {
    //TODO
}

void Lattice::appendString(std::string text) {
    allText_ += text;
    implicitOutEdges_.resize(allText_.length() + 1);
    std::string::iterator uti(text.begin());

    while (uti != text.end()) {

        uint32_t code_point = utf8::next(uti, text.end());

        std::string symbol;
        utf8::append(code_point, std::back_inserter(symbol));

        std::map<int, Graph::vertex_descriptor>::iterator mi = vertices_.end();
        --mi;

        int previousVertexIndex = (*mi).first;
        Graph::vertex_descriptor previousVertex = (*mi).second;
        Graph::vertex_descriptor vertex = boost::add_vertex(graph_);
        vertices_[previousVertexIndex + 1] = vertex;

        for (int j = 0; j < indexedTagCollections_.size(); ++j) {
            graph_[vertex].outEdgesIndex.push_back(std::list<EdgeDescriptor>());
            graph_[vertex].inEdgesIndex.push_back(std::list<EdgeDescriptor>());
        }

        addEdge(
            previousVertex,
            vertex,
            AnnotationItem(symbol),
            layerTagManager_.createSingletonTagCollection("raw"),
            0.0
        );

    }
}

Lattice::VertexDescriptor Lattice::getVertexForRawCharIndex(int ix) {
    return ix;
}

Lattice::VertexDescriptor Lattice::getFirstVertex() {
    return 0;
}

Lattice::VertexDescriptor Lattice::getLastVertex() {
    return allText_.length();
}

Lattice::EdgeDescriptor Lattice::addEdge(
    VertexDescriptor from,
    VertexDescriptor to,
    const AnnotationItem& annotationItem,
    LayerTagCollection tags,
    Score score,
    std::list<EdgeDescriptor> partition
) {

    std::pair<EdgeDescriptor, bool> result;

    std::pair<VertexDescriptor, VertexDescriptor> vpair(from, to);

    std::pair<
        std::pair<VertexDescriptor, VertexDescriptor>,
        std::pair<AnnotationItem, LayerTagCollection>
    > hkey(vpair, std::pair<AnnotationItem, LayerTagCollection>(annotationItem, tags));

    std::pair<VVCHash::iterator, bool> insertResult(vvcHash_.insert(
        std::pair<
            std::pair<
                std::pair<VertexDescriptor, VertexDescriptor>,
                std::pair<AnnotationItem, LayerTagCollection>
            >,
            EdgeDescriptor
        >(hkey, EdgeDescriptor())
    ));

    if (insertResult.second) {

        if (!edgeCounterHash_[vpair]) {
            edgeCounterHash_[vpair] = 1;
        } else {
            ++edgeCounterHash_[vpair];
        }

        Graph::vertex_descriptor boost_from = vertices_[from];
        Graph::vertex_descriptor boost_to   = vertices_[to];

        result = boost::add_edge(
            boost_from,
            boost_to,
            EdgeEntry(annotationItem, tags, score, partition),
            graph_
        );

        if (result.second) {
            for (int i = 0; i < indexedTagCollections_.size(); ++i) {
                if (createIntersection(tags, indexedTagCollections_.right.at(i)).isNonempty()) {
                    graph_[boost_from].outEdgesIndex[i].push_back(result.first);
                    graph_[boost_to].inEdgesIndex[i].push_back(result.first);
                }
            }
            return result.first;
        }

    }

    throw NoEdgeException("No edge added.");

}

Lattice::InOutEdgesIterator Lattice::outEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    Graph::vertex_descriptor boost_vertex = vertices_[vertex];
    if (mask.isAny()) {
        return Lattice::InOutEdgesIterator(boost::out_edges(boost_vertex, graph_));
    }
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return Lattice::InOutEdgesIterator(
        graph_[boost_vertex].outEdgesIndex[ix].begin(),
        graph_[boost_vertex].outEdgesIndex[ix].end()
    );
}

Lattice::InOutEdgesIterator Lattice::inEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    Graph::vertex_descriptor boost_vertex = vertices_[vertex];
    if (mask.isAny()) {
        return Lattice::InOutEdgesIterator(boost::in_edges(boost_vertex, graph_));
    }
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return Lattice::InOutEdgesIterator(
        graph_[boost_vertex].inEdgesIndex[ix].begin(),
        graph_[boost_vertex].inEdgesIndex[ix].end()
    );
}

Lattice::InOutEdgesIterator Lattice::allOutEdges(
    Lattice::VertexDescriptor vertex
) {
    return outEdges(vertex, layerTagManager_.anyTag());
}

Lattice::InOutEdgesIterator Lattice::allInEdges(
    Lattice::VertexDescriptor vertex
) {
    return inEdges(vertex, layerTagManager_.anyTag());
}

Lattice::EdgeDescriptor Lattice::firstOutEdge(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (mask.isAny()) return *(boost::out_edges(vertices_[vertex], graph_).first);
    if (outEdges(vertex, mask).hasNext()) return outEdges(vertex, mask).next();
    throw NoEdgeException("No out-edges found.");
}

Lattice::EdgeDescriptor Lattice::firstInEdge(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (mask.isAny()) return *(boost::in_edges(vertices_[vertex], graph_).first);
    if (inEdges(vertex, mask).hasNext()) return inEdges(vertex, mask).next();
    throw NoEdgeException("No in-edges found.");
}

Lattice::SortedEdgesIterator Lattice::edgesSorted(LayerTagMask mask) {
    return Lattice::SortedEdgesIterator(this, mask);
}

Lattice::SortedEdgesIterator Lattice::allEdgesSorted() {
    return edgesSorted(layerTagManager_.anyTag());
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

int Lattice::addTagCollectionIndex_(LayerTagCollection tags) {
    TagCollectionsBimapLeftIterator li = indexedTagCollections_.left.find(tags);
    if (li != indexedTagCollections_.left.end()) {
        return li->second;
    } else {
        int ix = indexedTagCollections_.size();
        for (
            std::map<int, Graph::vertex_descriptor>::const_iterator vi = vertices_.begin();
            vi != vertices_.end();
            ++vi
        ) {
            graph_[(*vi).second].outEdgesIndex.push_back(std::list<EdgeDescriptor>());
            std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator>
                oeir = boost::out_edges((*vi).second, graph_);
            for (Lattice::OutEdgeIterator oei = oeir.first; oei != oeir.second; ++oei) {
                if (createIntersection(graph_[*oei].tagList, tags).isNonempty()) {
                    graph_[(*vi).second].outEdgesIndex[ix].push_back(*oei);
                }
            }

            graph_[(*vi).second].inEdgesIndex.push_back(std::list<EdgeDescriptor>());
            std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator>
                ieir = boost::in_edges((*vi).second, graph_);
            for (Lattice::InEdgeIterator iei = ieir.first; iei != ieir.second; ++iei) {
                if (createIntersection(graph_[*iei].tagList, tags).isNonempty()) {
                    graph_[(*vi).second].inEdgesIndex[ix].push_back(*iei);
                }
            }
        }
        indexedTagCollections_.insert(TagCollectionsBimapItem(tags, ix));
        return ix;
    }
}


bool Lattice::InOutEdgesIterator::hasNext() {
    switch (type_) {
    case EDGE_DESCRIPTOR_ITER : return edi_ != ediEnd_;
    case OUT_EDGE_ITER : return oei_ != oeiEnd_;
    case IN_EDGE_ITER : return iei_ != ieiEnd_;
    }
}

const std::string& Lattice::getAllText() const {
    return allText_;
}

Lattice::EdgeDescriptor Lattice::InOutEdgesIterator::next() {
    switch (type_) {
    case EDGE_DESCRIPTOR_ITER :
        if (edi_ != ediEnd_) return *(edi_++);
        break;
    case OUT_EDGE_ITER :
        if (oei_ != oeiEnd_) return *(oei_++);
        break;
    case IN_EDGE_ITER :
        if (iei_ != ieiEnd_) return *(iei_++);
        break;
    }
    throw NoEdgeException("Iterator has no next edges.");
}


Lattice::SortedEdgesIterator::SortedEdgesIterator(
    Lattice * lattice,
    LayerTagMask mask
) :
    lattice_(lattice),
    mask_(mask),
    vi_(lattice->vertices_.begin()),
    viEnd_(lattice->vertices_.end()),
    ei_(lattice->outEdges(0, mask))
{ }

bool Lattice::SortedEdgesIterator::hasNext() {
    if (ei_.hasNext()) return true;
    ++vi_;
    while (vi_ != viEnd_) {
        ei_ = lattice_->outEdges((*vi_).second, mask_);
        if (ei_.hasNext()) return true;
        ++vi_;
    }
    return false;
}

Lattice::EdgeDescriptor Lattice::SortedEdgesIterator::next() {
    if (ei_.hasNext()) return ei_.next();
    ++vi_;
    while (vi_ != viEnd_) {
        ei_ = lattice_->outEdges((*vi_).second, mask_);
        if (ei_.hasNext()) return ei_.next();
        ++vi_;
    }
    throw NoEdgeException("Iterator has no next edges.");
}




#include "lattice.hpp"

Lattice::Lattice(std::string text) : layerTagManager_() {
    Lattice::VertexDescriptor vertex = boost::add_vertex(graph_);
    vertices_.push_back(vertex);
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
    UTF8String utext(text);
    for (UTF8String::iterator uti = utext.begin(); uti != utext.end(); ++uti) {

        Lattice::VertexDescriptor previousVertex = vertices_.back();
        Lattice::VertexDescriptor vertex = boost::add_vertex(graph_);
        vertices_.push_back(vertex);

        for (int j = 0; j < indexedTagCollections_.size(); ++j) {
            graph_[vertex].outEdgesIndex.push_back(std::list<EdgeDescriptor>());
            graph_[vertex].inEdgesIndex.push_back(std::list<EdgeDescriptor>());
        }

        addEdge(
            previousVertex,
            vertex,
            AnnotationItem(*uti),
            layerTagManager_.createSingletonTagCollection("raw"),
            0.0
        );

    }
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

        result = boost::add_edge(
            from,
            to,
            EdgeEntry(annotationItem, tags, score, partition),
            graph_
        );

        if (result.second) {
            for (int i = 0; i < indexedTagCollections_.size(); ++i) {
                if (createIntersection(tags, indexedTagCollections_.right.at(i)).isNonempty()) {
                    graph_[from].outEdgesIndex[i].push_back(result.first);
                    graph_[to].inEdgesIndex[i].push_back(result.first);
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
    if (mask.isAny()) {
        return Lattice::InOutEdgesIterator(boost::out_edges(vertex, graph_));
    }
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return Lattice::InOutEdgesIterator(
        graph_[vertex].outEdgesIndex[ix].begin(),
        graph_[vertex].outEdgesIndex[ix].end()
    );
}

Lattice::InOutEdgesIterator Lattice::inEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (mask.isAny()) {
        return Lattice::InOutEdgesIterator(boost::in_edges(vertex, graph_));
    }
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return Lattice::InOutEdgesIterator(
        graph_[vertex].inEdgesIndex[ix].begin(),
        graph_[vertex].inEdgesIndex[ix].end()
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
    if (mask.isAny()) return *(boost::out_edges(vertex, graph_).first);
    if (outEdges(vertex, mask).hasNext()) return outEdges(vertex, mask).next();
    throw NoEdgeException("No out-edges found.");
}

Lattice::EdgeDescriptor Lattice::firstInEdge(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (mask.isAny()) return *(boost::in_edges(vertex, graph_).first);
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
            std::vector<VertexDescriptor>::const_iterator vi = vertices_.begin();
            vi != vertices_.end();
            ++vi
        ) {
            graph_[*vi].outEdgesIndex.push_back(std::list<EdgeDescriptor>());
            std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator>
                oeir = boost::out_edges(*vi, graph_);
            for (Lattice::OutEdgeIterator oei = oeir.first; oei != oeir.second; ++oei) {
                if (createIntersection(graph_[*oei].tagList, tags).isNonempty()) {
                    graph_[*vi].outEdgesIndex[ix].push_back(*oei);
                }
            }

            graph_[*vi].inEdgesIndex.push_back(std::list<EdgeDescriptor>());
            std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator>
                ieir = boost::in_edges(*vi, graph_);
            for (Lattice::InEdgeIterator iei = ieir.first; iei != ieir.second; ++iei) {
                if (createIntersection(graph_[*iei].tagList, tags).isNonempty()) {
                    graph_[*vi].inEdgesIndex[ix].push_back(*iei);
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
    ei_(lattice->outEdges(*(lattice->vertices_.begin()), mask))
{ }

bool Lattice::SortedEdgesIterator::hasNext() {
    return vi_ != viEnd_ || ei_.hasNext();
}

Lattice::EdgeDescriptor Lattice::SortedEdgesIterator::next() {
    if (ei_.hasNext()) return ei_.next();
    while (vi_ != viEnd_) {
        ++vi_;
        ei_ = lattice_->outEdges(*vi_, mask_);
        if (ei_.hasNext()) return ei_.next();
    }
    throw NoEdgeException("Iterator has no next edges.");
}

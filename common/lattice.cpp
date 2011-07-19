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

Lattice::LatticeEdgeIterator Lattice::outEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    Lattice::LatticeEdgeIterator result;
    for (
        Lattice::EdgeDescriptorIterator edi = graph_[vertex].outEdgesIndex[ix].begin();
        edi != graph_[vertex].outEdgesIndex[ix].end();
        ++edi
    ) {
        result.add(*edi);
    }
    return result;
}

Lattice::LatticeEdgeIterator Lattice::inEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    Lattice::LatticeEdgeIterator result;
    for (
        Lattice::EdgeDescriptorIterator edi = graph_[vertex].inEdgesIndex[ix].begin();
        edi != graph_[vertex].inEdgesIndex[ix].end();
        ++edi
    ) {
        result.add(*edi);
    }
    return result;
}

Lattice::LatticeEdgeIterator Lattice::allOutEdges(
    Lattice::VertexDescriptor vertex
) {
    return boost::out_edges(vertex, graph_);
}

Lattice::LatticeEdgeIterator Lattice::allInEdges(
    Lattice::VertexDescriptor vertex
) {
    return boost::in_edges(vertex, graph_);
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

Lattice::LatticeEdgeIterator Lattice::edgesSorted(LayerTagMask mask) {
    Lattice::LatticeEdgeIterator result;
    for (
        std::vector<Lattice::VertexDescriptor>::iterator vi = vertices_.begin();
        vi != vertices_.end();
        ++vi
    ) {
        Lattice::LatticeEdgeIterator lei = outEdges(*vi, mask);
        while (lei.hasNext()) {
            result.add(lei.next());
        }
    }
    return result;
}

Lattice::LatticeEdgeIterator Lattice::allEdgesSorted() {
    Lattice::LatticeEdgeIterator result;
    for (
        std::vector<Lattice::VertexDescriptor>::iterator vi = vertices_.begin();
        vi != vertices_.end();
        ++vi
    ) {
        Lattice::LatticeEdgeIterator lei = allOutEdges(*vi);
        while (lei.hasNext()) {
            result.add(lei.next());
        }
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


Lattice::LatticeEdgeIterator::LatticeEdgeIterator(
    std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator> oeir
) {
    for (Lattice::OutEdgeIterator oei = oeir.first; oei != oeir.second; ++oei) {
        l_.push_back(*oei);
    }
    i_ = l_.begin();
}

Lattice::LatticeEdgeIterator::LatticeEdgeIterator(
    std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator> ieir
) {
    for (Lattice::InEdgeIterator iei = ieir.first; iei != ieir.second; ++iei) {
        l_.push_back(*iei);
    }
    i_ = l_.begin();
}

Lattice::LatticeEdgeIterator::LatticeEdgeIterator() {
    i_ = l_.begin();
}

bool Lattice::LatticeEdgeIterator::hasNext() {
    return i_ != l_.end();
}

Lattice::EdgeDescriptor Lattice::LatticeEdgeIterator::next() {
    return *(i_++);
}

void Lattice::LatticeEdgeIterator::remove() {
    l_.erase(i_);
}

void Lattice::LatticeEdgeIterator::add(Lattice::EdgeDescriptor edge) {
    l_.push_back(edge);
    i_ = l_.begin();
}
#include "lattice.hpp"

Lattice::Lattice() { }

Lattice::Lattice(std::string text) {
    appendString(text);
}

Lattice::~Lattice() { }

void Lattice::appendString(std::string text) {
    allText_ += text;
    implicitOutEdges_.resize(allText_.length() + 1);
    hiddenImplicitOutEdges_.resize(allText_.length() + 1);
}

void Lattice::addSymbols(VertexDescriptor startVertex, VertexDescriptor endVertex) {
    std::string::iterator iter = allText_.begin() + startVertex;
    std::string::iterator end = allText_.begin() + endVertex;
    VertexDescriptor vd = startVertex;
    while (iter != end) {
        std::string symbol;
        utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
        addEdge(
            vd,
            vd + symbol.length(),
            AnnotationItem(symbol),
            layerTagManager_.createSingletonTagCollection("symbol")
        );
        vd += symbol.length();
    }
}

void Lattice::appendStringWithSymbols(std::string text) {
    Lattice::VertexDescriptor joinPoint = getLastVertex();
    appendString(text);
    addSymbols(joinPoint, getLastVertex());
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
    Partition partition
) {

    std::pair<Graph::edge_descriptor, bool> result;

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

        if (tags == layerTagManager_.createSingletonTagCollection("symbol")) {
            implicitOutEdges_.set(from, true);
            return EdgeDescriptor(from);
        }

        Graph::vertex_descriptor boost_from;
        Graph::vertex_descriptor boost_to;

        std::map<int, Graph::vertex_descriptor>::iterator iterFrom = vertices_.find(from);
        std::map<int, Graph::vertex_descriptor>::iterator iterTo = vertices_.find(to);

        if (iterFrom != vertices_.end()) {
            boost_from = (*iterFrom).second;
        } else {
            boost_from = boost::add_vertex(VertexEntry(from), graph_);
            vertices_[from] = boost_from;
            for (int i = 0; i < indexedTagCollections_.size(); ++i) {
                graph_[boost_from].outEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
                graph_[boost_from].inEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
            }
        }

        if (iterTo != vertices_.end()) {
            boost_to = (*iterFrom).second;
        } else {
            boost_to = boost::add_vertex(VertexEntry(to), graph_);
            vertices_[to] = boost_to;
            for (int i = 0; i < indexedTagCollections_.size(); ++i) {
                graph_[boost_to].outEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
                graph_[boost_to].inEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
            }
        }

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
            for (VertexDescriptor vd = from; vd < to; ++vd) {
                hiddenImplicitOutEdges_[vd] = true;
            }
            return EdgeDescriptor(result.first);
        }

    }

    throw NoEdgeException("No edge added.");

}

Lattice::InOutEdgesIterator Lattice::outEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    std::map<int, Graph::vertex_descriptor>::iterator iter = vertices_.find(vertex);
    if (iter == vertices_.end()) {
        return Lattice::InOutEdgesIterator(
            (layerTagManager_.match(mask, "symbol") && implicitOutEdges_[vertex]) ?
                vertex : -1
        );
    }
    Graph::vertex_descriptor boost_vertex = (*iter).second;
    if (mask.isAny()) {
        return Lattice::InOutEdgesIterator(
            boost::out_edges(boost_vertex, graph_),
            implicitOutEdges_[vertex] ? vertex : -1
        );
    }
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return Lattice::InOutEdgesIterator(
        graph_[boost_vertex].outEdgesIndex[ix].begin(),
        graph_[boost_vertex].outEdgesIndex[ix].end(),
        (layerTagManager_.match(mask, "symbol") && implicitOutEdges_[vertex]) ?
            vertex : -1
    );
}

Lattice::InOutEdgesIterator Lattice::inEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    VertexDescriptor priorVertex = priorVertex_(vertex);
    std::map<int, Graph::vertex_descriptor>::iterator iter = vertices_.find(vertex);
    if (iter == vertices_.end()) {
        return Lattice::InOutEdgesIterator(
            (layerTagManager_.match(mask, "symbol") && implicitOutEdges_[priorVertex]) ?
                priorVertex : -1
        );
    }
    Graph::vertex_descriptor boost_vertex = (*iter).second;
    if (mask.isAny()) {
        return Lattice::InOutEdgesIterator(
            boost::in_edges(boost_vertex, graph_),
            implicitOutEdges_[priorVertex] ? priorVertex : -1
        );
    }
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return Lattice::InOutEdgesIterator(
        graph_[boost_vertex].inEdgesIndex[ix].begin(),
        graph_[boost_vertex].inEdgesIndex[ix].end(),
        (layerTagManager_.match(mask, "symbol") && implicitOutEdges_[priorVertex]) ?
            priorVertex : -1
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
    if (outEdges(vertex, mask).hasNext()) return outEdges(vertex, mask).next();
    throw NoEdgeException("No out-edges found.");
}

Lattice::EdgeDescriptor Lattice::firstInEdge(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
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

AnnotationItemManager& Lattice::getAnnotationItemManager() {
    return annotationItemManager_;
}

const AnnotationItem Lattice::getEdgeAnnotationItem(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) {
        return graph_[edge.descriptor].category;
    }
    std::string::iterator iter = allText_.begin() + edge.implicitIndex;
    std::string::iterator end = allText_.end();
    std::string symbol;
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return AnnotationItem(symbol);
}

const LayerTagCollection Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) {
        return graph_[edge.descriptor].tagList;
    }
    return layerTagManager_.createSingletonTagCollection("symbol");
}

int Lattice::getEdgeBeginIndex(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) {
        return graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return edge.implicitIndex;
}

int Lattice::getEdgeLength(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) {
        return graph_[boost::target(edge.descriptor, graph_)].index
            - graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return edge.implicitIndex - priorVertex_(edge.implicitIndex);
}

bool Lattice::isEdgeHidden(Lattice::EdgeDescriptor edge) {
    return edge.implicitIndex > -1 && hiddenImplicitOutEdges_[edge.implicitIndex];
}

const std::string& Lattice::getAllText() const {
    return allText_;
}

void Lattice::runCutter(Cutter& cutter, LayerTagMask mask) {
    // TODO
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
            graph_[(*vi).second].outEdgesIndex.push_back(std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>());
            std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator>
                oeir = boost::out_edges((*vi).second, graph_);
            for (Lattice::OutEdgeIterator oei = oeir.first; oei != oeir.second; ++oei) {
                if (createIntersection(graph_[*oei].tagList, tags).isNonempty()) {
                    graph_[(*vi).second].outEdgesIndex[ix].push_back(*oei);
                }
            }

            graph_[(*vi).second].inEdgesIndex.push_back(std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>());
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

Lattice::VertexDescriptor Lattice::priorVertex_(Lattice::VertexDescriptor vertex) {
    std::string::iterator begin = allText_.begin();
    std::string::iterator iter = begin + vertex;
    std::string symbol;
    utf8::append(utf8::prior(iter, begin), std::back_inserter(symbol));
    return vertex - symbol.length();
}



bool Lattice::VertexIterator::hasNext() {
    while (vd_ < lattice_->allText_.length()) {
        if (
            lattice_->vertices_.find(vd_) != lattice_->vertices_.end()
            || lattice_->implicitOutEdges_[vd_]
            || lattice_->implicitOutEdges_[lattice_->priorVertex_(vd_)]
        ) {
            return true;
        }
        ++vd_;
    }
    return false;
}

Lattice::VertexDescriptor Lattice::VertexIterator::next() {
    while (vd_ < lattice_->allText_.length()) {
        if (
            lattice_->vertices_.find(vd_) != lattice_->vertices_.end()
            || lattice_->implicitOutEdges_[vd_]
            || lattice_->implicitOutEdges_[lattice_->priorVertex_(vd_)]
        ) {
            return vd_++;
        }
        ++vd_;
    }
    throw NoEdgeException("Iterator has no next edges.");
}



bool Lattice::InOutEdgesIterator::hasNext() {
    if (implicitIndex_ > -1) return true;
    switch (type_) {
    case EDGE_DESCRIPTOR_ITER : return edi_ != ediEnd_;
    case OUT_EDGE_ITER : return oei_ != oeiEnd_;
    case IN_EDGE_ITER : return iei_ != ieiEnd_;
    case IMPLICIT_ITER : return implicitIndex_ > -1;
    }
}

Lattice::EdgeDescriptor Lattice::InOutEdgesIterator::next() {
    if (implicitIndex_ > -1) {
        int index = implicitIndex_;
        implicitIndex_ = -1;
        return EdgeDescriptor(index);
    }
    switch (type_) {
    case EDGE_DESCRIPTOR_ITER :
        if (edi_ != ediEnd_) return EdgeDescriptor(*(edi_++));
        break;
    case OUT_EDGE_ITER :
        if (oei_ != oeiEnd_) return EdgeDescriptor(*(oei_++));
        break;
    case IN_EDGE_ITER :
        if (iei_ != ieiEnd_) return EdgeDescriptor(*(iei_++));
        break;
    case IMPLICIT_ITER :
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
    vi_(lattice),
    ei_(lattice->outEdges(0, mask))
{
    vi_.next();
}

bool Lattice::SortedEdgesIterator::hasNext() {
    if (ei_.hasNext()) return true;
    while (vi_.hasNext()) {
        ei_ = lattice_->outEdges(vi_.next(), mask_);
        if (ei_.hasNext()) return true;
    }
    return false;
}

Lattice::EdgeDescriptor Lattice::SortedEdgesIterator::next() {
    if (ei_.hasNext()) return ei_.next();
    while (vi_.hasNext()) {
        ei_ = lattice_->outEdges(vi_.next(), mask_);
        if (ei_.hasNext()) return ei_.next();
    }
    throw NoEdgeException("Iterator has no next edges.");
}




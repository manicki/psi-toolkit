#include "lattice.hpp"

Lattice::Lattice(std::string text) : implicitOutEdges_(text.length() + 1) {
    Graph::vertex_descriptor vertex = boost::add_vertex(VertexEntry(0), graph_);
    vertices_[0] = vertex;
    for (int j = 0; j < indexedTagCollections_.size(); ++j) {
        graph_[vertex].outEdgesIndex.push_back(std::list<Graph::edge_descriptor>());
        graph_[vertex].inEdgesIndex.push_back(std::list<Graph::edge_descriptor>());
    }
    appendString(text);
}

Lattice::~Lattice() { }

void Lattice::appendString(std::string text) {
    allText_ += text;
    implicitOutEdges_.resize(allText_.length() + 1);
}

void Lattice::addSymbols(VertexDescriptor startVertex, VertexDescriptor endVertex) {
    std::string text = allText_.substr(startVertex, endVertex-startVertex);
    std::string::iterator uti(text.begin());
    while (uti != text.end()) {

        uint32_t code_point = utf8::next(uti, text.end());

        std::string symbol;
        utf8::append(code_point, std::back_inserter(symbol));

        std::map<int, Graph::vertex_descriptor>::iterator mi = vertices_.end();
        --mi;

        int previousVertexIndex = (*mi).first;
        int vertexIndex = previousVertexIndex + symbol.length();
        Graph::vertex_descriptor previousVertex = (*mi).second;
        Graph::vertex_descriptor vertex = boost::add_vertex(VertexEntry(vertexIndex), graph_);
        vertices_[vertexIndex] = vertex;

        for (int j = 0; j < indexedTagCollections_.size(); ++j) {
            graph_[vertex].outEdgesIndex.push_back(std::list<Graph::edge_descriptor>());
            graph_[vertex].inEdgesIndex.push_back(std::list<Graph::edge_descriptor>());
        }

        addEdge(
            previousVertex,
            vertex,
            AnnotationItem(symbol),
            layerTagManager_.createSingletonTagCollection("symbol"),
            0.0
        );

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
    std::list<EdgeDescriptor> partition
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
            return EdgeDescriptor(result.first);
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
    Graph::vertex_descriptor boost_vertex = vertices_[vertex];
    VertexDescriptor priorVertex = priorVertex_(vertex);
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
    if (mask.isAny()) return EdgeDescriptor(*(boost::out_edges(vertices_[vertex], graph_).first));
    if (outEdges(vertex, mask).hasNext()) return outEdges(vertex, mask).next();
    throw NoEdgeException("No out-edges found.");
}

Lattice::EdgeDescriptor Lattice::firstInEdge(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (mask.isAny()) return EdgeDescriptor(*(boost::in_edges(vertices_[vertex], graph_).first));
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

const AnnotationItem Lattice::getEdgeAnnotationItem(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) return graph_[edge.descriptor].category;
    std::string::iterator iter = allText_.begin() + edge.implicitIndex;
    std::string::iterator end = allText_.end();
    std::string symbol;
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return AnnotationItem(symbol);
}

const LayerTagCollection Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) return graph_[edge.descriptor].tagList;
    return layerTagManager_.createSingletonTagCollection("symbol");
}

const std::string& Lattice::getAllText() const {
    return allText_;
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
            graph_[(*vi).second].outEdgesIndex.push_back(std::list<Graph::edge_descriptor>());
            std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator>
                oeir = boost::out_edges((*vi).second, graph_);
            for (Lattice::OutEdgeIterator oei = oeir.first; oei != oeir.second; ++oei) {
                if (createIntersection(graph_[*oei].tagList, tags).isNonempty()) {
                    graph_[(*vi).second].outEdgesIndex[ix].push_back(*oei);
                }
            }

            graph_[(*vi).second].inEdgesIndex.push_back(std::list<Graph::edge_descriptor>());
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
            lattice_->vertices_[vd_]
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
            lattice_->vertices_[vd_]
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




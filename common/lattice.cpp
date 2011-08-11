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

        if (
            tags == layerTagManager_.createSingletonTagCollection("symbol")
            && from + (int) symbolLength_(from) == to
        ) {
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
            for (size_t i = 0; i < indexedTagCollections_.size(); ++i) {
                graph_[boost_from].outEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
                graph_[boost_from].inEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
            }
        }

        if (iterTo != vertices_.end()) {
            boost_to = (*iterTo).second;
        } else {
            boost_to = boost::add_vertex(VertexEntry(to), graph_);
            vertices_[to] = boost_to;
            for (size_t i = 0; i < indexedTagCollections_.size(); ++i) {
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
            for (size_t i = 0; i < indexedTagCollections_.size(); ++i) {
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
    if (vertex < 1) {
        return InOutEdgesIterator();
    }
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

Lattice::EdgesSortedBySourceIterator Lattice::edgesSortedBySource(LayerTagMask mask) {
    return Lattice::EdgesSortedBySourceIterator(this, mask);
}

Lattice::EdgesSortedBySourceIterator Lattice::allEdgesSortedBySource() {
    return edgesSortedBySource(layerTagManager_.anyTag());
}

Lattice::EdgesSortedByTargetIterator Lattice::edgesSortedByTarget(LayerTagMask mask) {
    return Lattice::EdgesSortedByTargetIterator(this, mask);
}

Lattice::EdgesSortedByTargetIterator Lattice::allEdgesSortedByTarget() {
    return edgesSortedByTarget(layerTagManager_.anyTag());
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
    std::string symbol = "'";
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return AnnotationItem(symbol);
}

const LayerTagCollection Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) {
        return graph_[edge.descriptor].tagList;
    }
    return layerTagManager_.createSingletonTagCollection("symbol");
}

int Lattice::getEdgeBeginIndex(Lattice::EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return edge.implicitIndex;
}

int Lattice::getEdgeLength(Lattice::EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return graph_[boost::target(edge.descriptor, graph_)].index
            - graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return symbolLength_(edge.implicitIndex);
}

bool Lattice::isEdgeHidden(Lattice::EdgeDescriptor edge) const {
    return edge.implicitIndex > -1 && hiddenImplicitOutEdges_[edge.implicitIndex];
}

std::list<Lattice::Partition> Lattice::getEdgePartitions(Lattice::EdgeDescriptor edge) {
    if (edge.implicitIndex < 0) {
        return graph_[edge.descriptor].partitions;
    }
    return std::list<Lattice::Partition>();
}

Lattice::Score Lattice::getEdgeScore(Lattice::EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return graph_[edge.descriptor].score;
    }
    return 0.0;
}

Lattice::VertexDescriptor Lattice::getEdgeSource(EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return VertexDescriptor(boost::source(edge.descriptor, graph_));
    }

    return VertexDescriptor(edge.implicitIndex);
}

Lattice::VertexDescriptor Lattice::getEdgeTarget(EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return VertexDescriptor(boost::target(edge.descriptor, graph_));
    }

    return VertexDescriptor(edge.implicitIndex + symbolLength_(edge.implicitIndex));
}


const std::string& Lattice::getAllText() const {
    return allText_;
}

const std::string Lattice::getEdgeText(EdgeDescriptor edge) const {
    return allText_.substr(getEdgeBeginIndex(edge), getEdgeLength(edge));
}

const std::string Lattice::getPartitionText(Partition partition) const {
    std::string r;

    for(std::vector<EdgeDescriptor>::iterator it = partition.links.begin();
        it != partition.links.end();
        ++it)
        r += getEdgeText(*it);

    return r;
}


void Lattice::runCutter(Cutter& cutter, LayerTagMask mask) {
    VertexDescriptor vertex = getFirstVertex();

    Partition partition = getPath(vertex, mask);

    std::string text = getPartitionText(partition);

    size_t pos = 0;
    std::vector<EdgeDescriptor>::const_iterator partitionIter = partition.links.begin();

    LayerTagCollection tags = layerTagManager_.createTagCollection(cutter.layerTags());

    do {
        size_t prevPos = pos;

        AnnotationItem item = cutter.cutOff(text, pos);

        int itemLength = (pos == std::string::npos ? text.length() : pos) - prevPos;

        Partition itemPartition = cutPartitionByTextLength_(partition, partitionIter, itemLength);

        addEdge(firstPartitionVertex_(itemPartition),
                lastPartitionVertex_(itemPartition),
                item,
                tags,
                0.0,
                itemPartition);

    } while (pos != std::string::npos);
}

Lattice::Partition Lattice::getPath(VertexDescriptor& vertex, LayerTagMask mask) {
    bool nextVertexFound = true;

    Lattice::Partition path;

    do {
        InOutEdgesIterator iter = outEdges(vertex, mask);

        if (iter.hasNext()) {
            EdgeDescriptor bestOne = iter.next();

            while (iter.hasNext())
                bestOne = iter.next();

            path.addEdge(bestOne);
            vertex = getEdgeTarget(bestOne);
        } else
            nextVertexFound = false;

    } while(nextVertexFound);

    return path;
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
    if (vertex < 1) {
        throw NoVertexException("Beginning vertex has no prior vertex.");
    }
    std::string::iterator begin = allText_.begin();
    std::string::iterator iter = begin + vertex;
    std::string symbol;
    utf8::append(utf8::prior(iter, begin), std::back_inserter(symbol));
    return vertex - symbol.length();
}

size_t Lattice::symbolLength_(int ix) const {
    std::string::const_iterator iter = allText_.begin() + ix;
    std::string::const_iterator end = allText_.end();
    std::string symbol;
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return symbol.length();
}

Lattice::VertexDescriptor Lattice::firstPartitionVertex_(const Partition& partition) const {
    return getEdgeSource(partition.firstEdge());
}

Lattice::VertexDescriptor Lattice::lastPartitionVertex_(const Partition& partition) const {
    return getEdgeTarget(partition.lastEdge());
}

Lattice::Partition Lattice::cutPartitionByTextLength_(const Partition& partition,
                                                      std::vector<EdgeDescriptor>::const_iterator& partitionIterator,
                                                      int length) {
    int lengthGathered = 0;

    Partition ret;

    for(;
        partitionIterator != partition.links.end() && lengthGathered < length;
        lengthGathered += getEdgeText(*partitionIterator).length(), ++partitionIterator)
        ret.links.push_back(*partitionIterator);

    return ret;
}


bool Lattice::VertexIterator::hasNext() {
    while (vd_ <= (int)lattice_->allText_.length()) {
        if (
            lattice_->vertices_.find(vd_) != lattice_->vertices_.end()
            || lattice_->implicitOutEdges_[vd_]
            || (vd_ > 0 && lattice_->implicitOutEdges_[lattice_->priorVertex_(vd_)])
        ) {
            return true;
        }
        ++vd_;
    }
    return false;
}

Lattice::VertexDescriptor Lattice::VertexIterator::next() {
    while (vd_ <= (int)lattice_->allText_.length()) {
        if (
            lattice_->vertices_.find(vd_) != lattice_->vertices_.end()
            || lattice_->implicitOutEdges_[vd_]
            || (vd_ > 0 && lattice_->implicitOutEdges_[lattice_->priorVertex_(vd_)])
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

    throw NoEdgeException("unexpected state");
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
    vi_(lattice)
{
    vi_.next();
}

bool Lattice::SortedEdgesIterator::hasNext() {
    if (ei_.hasNext()) return true;
    while (vi_.hasNext()) {
        ei_ = getEdgesIterator_(vi_.next());
        if (ei_.hasNext()) return true;
    }
    return false;
}

Lattice::EdgeDescriptor Lattice::SortedEdgesIterator::next() {
    if (ei_.hasNext()) return ei_.next();
    while (vi_.hasNext()) {
        ei_ = getEdgesIterator_(vi_.next());
        if (ei_.hasNext()) return ei_.next();
    }
    throw NoEdgeException("Iterator has no next edges.");
}

Lattice::SortedEdgesIterator::~SortedEdgesIterator() {
}

Lattice::EdgesSortedBySourceIterator::EdgesSortedBySourceIterator(
    Lattice * lattice,
    LayerTagMask mask
) : SortedEdgesIterator(lattice, mask) {
    ei_ = lattice_->outEdges(0, mask_);
}

Lattice::InOutEdgesIterator Lattice::EdgesSortedBySourceIterator::getEdgesIterator_(
    Lattice::VertexDescriptor vd
) {
    return lattice_->outEdges(vd, mask_);
}

Lattice::EdgesSortedByTargetIterator::EdgesSortedByTargetIterator(
    Lattice * lattice,
    LayerTagMask mask
) : SortedEdgesIterator(lattice, mask) {
    ei_ = lattice_->inEdges(0, mask_);
}

Lattice::InOutEdgesIterator Lattice::EdgesSortedByTargetIterator::getEdgesIterator_(
    Lattice::VertexDescriptor vd
) {
    return lattice_->inEdges(vd, mask_);
}





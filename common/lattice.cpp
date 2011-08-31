#include "lattice.hpp"

Lattice::Lattice() :
    nLooseVertices_(0),
    symbolTag_(layerTagManager_.createSingletonTagCollection("symbol"))
{ }

Lattice::Lattice(std::string text) :
    nLooseVertices_(0),
    symbolTag_(layerTagManager_.createSingletonTagCollection("symbol"))
{
    appendString(text);
}

Lattice::~Lattice() { }

void Lattice::appendString(std::string text) {
    allText_ += text;
    implicitOutEdges_.resize(allText_.length() + 1);
    hiddenImplicitOutEdges_.resize(allText_.length() + 1);
}

void Lattice::addSymbols(VertexDescriptor startVertex, VertexDescriptor endVertex) {
    if (isLooseVertex(startVertex) || isLooseVertex(endVertex)) {
        throw WrongVertexException("Cannot add default symbol edges between loose vertices.");
    }
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
            getSymbolTag_());
        vd += symbol.length();
    }
}

void Lattice::appendStringWithSymbols(std::string text) {
    Lattice::VertexDescriptor joinPoint = getLastVertex();
    appendString(text);
    addSymbols(joinPoint, getLastVertex());
}

Lattice::VertexDescriptor Lattice::addLooseVertex() {
    ++nLooseVertices_;
    Graph::vertex_descriptor vertex = boost::add_vertex(VertexEntry(-nLooseVertices_), graph_);
    vertices_[-nLooseVertices_] = vertex;
    for (size_t i = 0; i < indexedTagCollections_.size(); ++i) {
        graph_[vertex].outEdgesIndex.push_back(
            std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
        );
        graph_[vertex].inEdgesIndex.push_back(
            std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
        );
    }
    return -nLooseVertices_;
}

Lattice::VertexDescriptor Lattice::getVertexForRawCharIndex(int ix) {
    if (ix < 0) {
        throw NoVertexException("Raw char index below zero is incorrect.");
    }
    if (ix > int(allText_.length())) {
        throw NoVertexException("Raw char index above text length is incorrect.");
    }
    return ix;
}

Lattice::VertexDescriptor Lattice::getFirstVertex() const {
    return 0;
}

Lattice::VertexDescriptor Lattice::getLastVertex() const {
    return allText_.length();
}

size_t Lattice::getVertexRawCharIndex(VertexDescriptor vd) {
    return size_t(vd);
}

Lattice::EdgeDescriptor Lattice::addEdge(
    VertexDescriptor from,
    VertexDescriptor to,
    const AnnotationItem& annotationItem,
    LayerTagCollection tags,
    EdgeSequence sequence,
    Score score,
    int ruleId)
{

    std::pair<Graph::edge_descriptor, bool> result;

    std::pair<VertexDescriptor, VertexDescriptor> vpair(from, to);

    std::pair<
        std::pair<VertexDescriptor, VertexDescriptor>,
        AnnotationItem
    > hkey(vpair, annotationItem);

    std::pair<VVCHash::iterator, bool> insertResult(vvcHash_.insert(
        std::pair<
            std::pair<
                std::pair<VertexDescriptor, VertexDescriptor>,
                AnnotationItem
            >,
            EdgeDescriptor
        >(hkey, EdgeDescriptor())
    ));

    bool needToAddEdge = false;

    if (insertResult.second) {
        needToAddEdge = true;
    } else {
        EdgeDescriptor edge = (insertResult.first)->second;
        LayerTagCollection oldTags = getEdgeLayerTags(edge);
        Score oldScore = getEdgeScore(edge);
        if (tags != oldTags) {
            tags = createUnion(oldTags, tags);
            if (edge.implicitIndex < 0) {
                graph_[edge.descriptor].tagList = tags;
            } else {
                implicitOutEdges_.set(from, false);
                edge.implicitIndex = -1;
                needToAddEdge = true;
            }
        }
        if (score > oldScore) {
            if (edge.implicitIndex < 0) {
                graph_[edge.descriptor].score = score;
            } else {
                implicitOutEdges_.set(from, false);
                edge.implicitIndex = -1;
                needToAddEdge = true;
            }
        }
        if (edge.implicitIndex < 0) {
            graph_[edge.descriptor].partitions.push_back(Partition(tags, sequence, score, ruleId));
        } else {
            implicitOutEdges_.set(from, false);
            edge.implicitIndex = -1;
            needToAddEdge = true;
        }
    }

    if (needToAddEdge) {

        if (!edgeCounterHash_[vpair]) {
            edgeCounterHash_[vpair] = 1;
        } else {
            ++edgeCounterHash_[vpair];
        }

        if (
            !isLooseVertex(from)
            && !isLooseVertex(to)
            && tags == getSymbolTag_()
            && from + (int) symbolLength_(from) == to
        ) {
            implicitOutEdges_.set(from, true);
            (insertResult.first)->second = EdgeDescriptor(from);
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
            EdgeEntry(annotationItem, tags, score, Partition(tags, sequence, score, ruleId)),
            graph_
        );

        if (result.second) {
            for (size_t i = 0; i < indexedTagCollections_.size(); ++i) {
                if (createIntersection(tags, indexedTagCollections_.right.at(i)).isNonempty()) {
                    graph_[boost_from].outEdgesIndex[i].push_back(result.first);
                    graph_[boost_to].inEdgesIndex[i].push_back(result.first);
                }
            }
            if (!isLooseVertex(from) && !isLooseVertex(to)) {
                for (VertexDescriptor vd = from; vd < to; ++vd) {
                    hiddenImplicitOutEdges_[vd] = true;
                }
            }
            (insertResult.first)->second = EdgeDescriptor(result.first);
            return EdgeDescriptor(result.first);
        }

    }

    return (insertResult.first)->second;
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
    if (vertex == 0) {
        return InOutEdgesIterator();
    }
    VertexDescriptor priorVertex;
    if (isLooseVertex(vertex)) {
        priorVertex = -1; // Variable unused because all loose vertices have explicit in-edges.
    } else {
        priorVertex = priorVertex_(vertex);
    }
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
    return Lattice::EdgesSortedBySourceIterator(*this, mask);
}

Lattice::EdgesSortedBySourceIterator Lattice::allEdgesSortedBySource() {
    return edgesSortedBySource(layerTagManager_.anyTag());
}

Lattice::EdgesSortedByTargetIterator Lattice::edgesSortedByTarget(LayerTagMask mask) {
    return Lattice::EdgesSortedByTargetIterator(*this, mask);
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
        return graph_[edge.descriptor].item;
    }
    std::string::iterator iter = allText_.begin() + edge.implicitIndex;
    std::string::iterator end = allText_.end();
    std::string symbol = "'";
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return AnnotationItem(symbol);
}

const LayerTagCollection& Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return graph_[edge.descriptor].tagList;
    }
    return getSymbolTag_();
}

int Lattice::getEdgeBeginIndex(Lattice::EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return edge.implicitIndex;
}

int Lattice::getEdgeEndIndex(Lattice::EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return graph_[boost::target(edge.descriptor, graph_)].index;
    }
    return edge.implicitIndex + symbolLength_(edge.implicitIndex);
}

int Lattice::getEdgeLength(Lattice::EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        if (isLooseVertex(getEdgeSource(edge)) || isLooseVertex(getEdgeTarget(edge))) {
            throw WrongVertexException("Edges linking loose vertices have no well-defined length");
        }
        return graph_[boost::target(edge.descriptor, graph_)].index
            - graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return symbolLength_(edge.implicitIndex);
}

bool Lattice::isEdgeHidden(Lattice::EdgeDescriptor edge) const {
    return edge.implicitIndex > -1 && hiddenImplicitOutEdges_[edge.implicitIndex];
}

std::list<Lattice::Partition> Lattice::getEdgePartitions(Lattice::EdgeDescriptor edge) const {
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
        return VertexDescriptor(graph_[boost::source(edge.descriptor, graph_)].index);
    }

    return VertexDescriptor(edge.implicitIndex);
}

Lattice::VertexDescriptor Lattice::getEdgeTarget(EdgeDescriptor edge) const {
    if (edge.implicitIndex < 0) {
        return VertexDescriptor(graph_[boost::target(edge.descriptor, graph_)].index);
    }

    return VertexDescriptor(edge.implicitIndex + symbolLength_(edge.implicitIndex));
}


const std::string& Lattice::getAllText() const {
    return allText_;
}

const std::string Lattice::getEdgeText(EdgeDescriptor edge) const {
    if (isLooseVertex(getEdgeSource(edge)) || isLooseVertex(getEdgeTarget(edge))) {
        return "@"; // temporarily for edges with loose vertices
    }
    return allText_.substr(getEdgeBeginIndex(edge), getEdgeLength(edge));
}

const std::string Lattice::getSequenceText(const EdgeSequence& sequence) const {
    std::string r;

    for(EdgeSequence::Iterator it = sequence.begin();
        it != sequence.end();
        ++it)
        r += getEdgeText(*it);

    return r;
}

const std::string Lattice::getPartitionText(const Partition& partition) const {
    return getSequenceText(partition.getSequence());
}

const std::string Lattice::getAnnotationText(EdgeDescriptor edge) {
    return getEdgeAnnotationItem(edge).getText();
}

const std::string Lattice::getAnnotationCategory(EdgeDescriptor edge) {
    return getEdgeAnnotationItem(edge).getCategory();
}


void Lattice::runCutter(Cutter& cutter, LayerTagMask mask) {
    VertexDescriptor vertex = getFirstVertex();

    EdgeSequence sequence = getPath(vertex, mask);

    if (sequence.empty())
        return;

    std::string text = getSequenceText(sequence);

    size_t pos = 0;
    EdgeSequence::Iterator sequenceIter = sequence.begin();

    LayerTagCollection tags = layerTagManager_.createTagCollection(cutter.layerTags());

    do {
        size_t prevPos = pos;

        AnnotationItem item = cutter.cutOff(text, pos);

        int itemLength = (pos == std::string::npos ? text.length() : pos) - prevPos;

        EdgeSequence itemSequence = cutSequenceByTextLength_(sequence, sequenceIter, itemLength);

        addEdge(firstSequenceVertex_(itemSequence),
                lastSequenceVertex_(itemSequence),
                item,
                tags,
                itemSequence);

    } while (pos != std::string::npos);
}

Lattice::EdgeSequence Lattice::getPath(VertexDescriptor& vertex, LayerTagMask mask) {
    bool nextVertexFound = true;

    Lattice::EdgeSequence::Builder pathBuilder;

    do {
        InOutEdgesIterator iter = outEdges(vertex, mask);

        if (iter.hasNext()) {
            EdgeDescriptor bestOne = iter.next();

            while (iter.hasNext())
                bestOne = iter.next();

            pathBuilder.addEdge(bestOne);
            vertex = getEdgeTarget(bestOne);
        } else
            nextVertexFound = false;

    } while(nextVertexFound);

    return pathBuilder.build();
}

bool Lattice::isLooseVertex(Lattice::VertexDescriptor vd) const {
    return vd < 0;
}

int Lattice::getLooseVertexIndex(VertexDescriptor vd) const {
    if (vd > -1) {
        throw WrongVertexException("Cannot get loose vertex index for a non-loose vertex");
    }
    return -1-vd;
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
    if (vertex == 0) {
        throw NoVertexException("Beginning vertex has no prior vertex.");
    }
    if (vertex < 0) {
        throw NoVertexException("Loose vertices have no prior vertex.");
    }
    std::string::iterator begin = allText_.begin();
    std::string::iterator iter = begin + vertex;
    std::string symbol;
    utf8::append(utf8::prior(iter, begin), std::back_inserter(symbol));
    return vertex - symbol.length();
}

void Lattice::orderLooseVertices_() {
    for (int i = 0; i <= allText_.length(); ++i) {
        if (lattice_.vertices_.find(i) != lattice_.vertices_.end()) {
            //TODO
        }
    }
}

size_t Lattice::symbolLength_(int ix) const {
    std::string::const_iterator iter = allText_.begin() + ix;
    std::string::const_iterator end = allText_.end();
    std::string symbol;
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return symbol.length();
}

const LayerTagCollection& Lattice::getSymbolTag_() const {
    return symbolTag_;
}

Lattice::EdgeSequence::EdgeSequence() {
}

Lattice::EdgeSequence::Iterator Lattice::EdgeSequence::begin() const {
    return links.begin();
}

Lattice::EdgeSequence::Iterator Lattice::EdgeSequence::end() const {
    return links.end();
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::firstEdge() const {
    return links.front();
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::lastEdge() const {
    return links.back();
}

bool Lattice::EdgeSequence::empty() const {
    return links.empty();
}

size_t Lattice::EdgeSequence::size() const {
    return links.size();
}

Lattice::EdgeSequence::Builder& Lattice::EdgeSequence::Builder::addEdge(EdgeDescriptor edge) {
    links.push_back(edge);

    return *this;
}

Lattice::EdgeSequence Lattice::EdgeSequence::Builder::build() {
    Lattice::EdgeSequence seq(links);

    return seq;
}

Lattice::EdgeSequence::EdgeSequence(const std::vector<EdgeDescriptor>& aLinks) {
    links = aLinks;
}


Lattice::Partition::Partition(LayerTagCollection aTagList,
                              EdgeSequence aSequence,
                              Score aScore,
                              int aRuleId):
    sequence_(aSequence), tagList_(aTagList), score_(aScore), ruleId_(aRuleId) {
}

size_t Lattice::Partition::size() const {
    return sequence_.size();
}


Lattice::Partition::Iterator Lattice::Partition::begin() const {
    return sequence_.begin();
}

Lattice::Partition::Iterator Lattice::Partition::end() const {
    return sequence_.end();
}


Lattice::EdgeDescriptor Lattice::Partition::firstEdge() const {
    return sequence_.firstEdge();
}

Lattice::EdgeDescriptor Lattice::Partition::lastEdge() const {
    return sequence_.lastEdge();
}

const Lattice::EdgeSequence& Lattice::Partition::getSequence() const {
    return sequence_;
}

Lattice::VertexDescriptor Lattice::firstSequenceVertex_(const EdgeSequence& sequence) const {
    return getEdgeSource(sequence.firstEdge());
}

Lattice::VertexDescriptor Lattice::lastSequenceVertex_(const EdgeSequence& sequence) const {
    return getEdgeTarget(sequence.lastEdge());
}

Lattice::VertexDescriptor Lattice::firstPartitionVertex_(const Partition& partition) const {
    return firstSequenceVertex_(partition.getSequence());
}

Lattice::VertexDescriptor Lattice::lastPartitionVertex_(const Partition& partition) const {
    return lastSequenceVertex_(partition.getSequence());
}

Lattice::EdgeSequence Lattice::cutSequenceByTextLength_(const EdgeSequence& sequence,
                                                    EdgeSequence::Iterator& sequenceIterator,
                                                    int length) {
    int lengthGathered = 0;

    EdgeSequence::Builder sequenceBuilder;

    for(;
        sequenceIterator != sequence.end() && lengthGathered < length;
        lengthGathered += getEdgeText(*sequenceIterator).length(), ++sequenceIterator)
        sequenceBuilder.addEdge(*sequenceIterator);

    return sequenceBuilder.build();
}


Lattice::VertexIterator::VertexIterator(Lattice& lattice) : lattice_(lattice), vd_(0) {
    if (lattice_.nLooseVertices_ > 0) {
        throw WrongVertexException("Iterating over loose vertices not implemented yet.");
    }
}

bool Lattice::VertexIterator::hasNext() {
    while (vd_ <= (int)lattice_.allText_.length()) {
        if (
            lattice_.vertices_.find(vd_) != lattice_.vertices_.end()
            || lattice_.implicitOutEdges_[vd_]
            || (vd_ > 0 && lattice_.implicitOutEdges_[lattice_.priorVertex_(vd_)])
        ) {
            return true;
        }
        nextRealVertex_();
    }
    return false;
}

Lattice::VertexDescriptor Lattice::VertexIterator::next() {
    while (vd_ <= (int)lattice_.allText_.length()) {
        if (
            lattice_.vertices_.find(vd_) != lattice_.vertices_.end()
            || lattice_.implicitOutEdges_[vd_]
            || (vd_ > 0 && lattice_.implicitOutEdges_[lattice_.priorVertex_(vd_)])
        ) {
            Lattice::VertexDescriptor returnedVertexDescriptor = vd_;
            nextRealVertex_();
            return returnedVertexDescriptor;
        }
        nextRealVertex_();
    }
    throw NoEdgeException("Vertex iterator has no next edges.");
}

void Lattice::VertexIterator::nextRealVertex_() {
    if (vd_ < (int)lattice_.allText_.length())
        vd_ += lattice_.symbolLength_((int)vd_);
    else
        ++vd_;
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
    throw NoEdgeException("InOutEdgesIterator has no next edges.");
}



Lattice::SortedEdgesIterator::SortedEdgesIterator(
    Lattice& lattice,
    LayerTagMask mask
) :
    lattice_(lattice),
    mask_(mask),
    vi_(lattice)
{
    if (vi_.hasNext())
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
    throw NoEdgeException("SortedEdgesIterator has no next edges.");
}

Lattice::SortedEdgesIterator::~SortedEdgesIterator() {
}

Lattice::EdgesSortedBySourceIterator::EdgesSortedBySourceIterator(
    Lattice& lattice,
    LayerTagMask mask
) : SortedEdgesIterator(lattice, mask) {
    ei_ = lattice_.outEdges(0, mask_);
}

Lattice::InOutEdgesIterator Lattice::EdgesSortedBySourceIterator::getEdgesIterator_(
    Lattice::VertexDescriptor vd
) {
    return lattice_.outEdges(vd, mask_);
}

Lattice::EdgesSortedByTargetIterator::EdgesSortedByTargetIterator(
    Lattice& lattice,
    LayerTagMask mask
) : SortedEdgesIterator(lattice, mask) {
    ei_ = lattice_.inEdges(0, mask_);
}

Lattice::InOutEdgesIterator Lattice::EdgesSortedByTargetIterator::getEdgesIterator_(
    Lattice::VertexDescriptor vd
) {
    return lattice_.inEdges(vd, mask_);
}





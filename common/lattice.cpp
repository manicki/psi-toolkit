#include "lattice.hpp"

#include "string_helpers.hpp"

Lattice::Lattice() :
    nLooseVertices_(0),
    symbolTag_(layerTagManager_.createSingletonTagCollection("symbol")),
    discardedTag_(layerTagManager_.createSingletonTagCollection("discarded")) {

    resizeImplicitEdgesStructures_();
}

Lattice::Lattice(const std::string & text) :
    nLooseVertices_(0),
    symbolTag_(layerTagManager_.createSingletonTagCollection("symbol")),
    discardedTag_(layerTagManager_.createSingletonTagCollection("discarded")) {

    appendString(text);
}

Lattice::~Lattice() { }

void Lattice::appendString(const std::string & text) {
    if (allText_.empty()) {
        allText_ = text;
    } else {
        allText_ += text;
    }

    resizeImplicitEdgesStructures_();
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
            AnnotationItem("'" + symbol),
            getSymbolTag_());
        vd += symbol.length();
    }
}

void Lattice::appendStringWithSymbols(const std::string & text) {
    Lattice::VertexDescriptor joinPoint = getLastVertex();
    appendString(text);
    addSymbols(joinPoint, getLastVertex());
}

Lattice::VertexDescriptor Lattice::addLooseVertex() {
    ++nLooseVertices_;
    Graph::vertex_descriptor vertex = boost::add_vertex(VertexEntry(-nLooseVertices_), graph_);
    vertices_[-nLooseVertices_] = vertex;
    BOOST_FOREACH(TagCollectionsBimapItem unused, indexedTagCollections_) {
        graph_[vertex].outEdgesIndex.push_back(
            std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
        );
        graph_[vertex].inEdgesIndex.push_back(
            std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
        );
    }
    return -nLooseVertices_;
}

Lattice::VertexDescriptor Lattice::getVertexForRawCharIndex(int ix) const {
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

    // doesn't check if a loose edge is reversed!
    if (from >= 0 && to >= 0 && from > to) {
        throw ReversedEdgeException("Cannot add a reversed edge");
    }

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
        EdgeSequence::Iterator sequenceIter(*this, sequence);
        while (sequenceIter.hasNext()) {
            if (sequenceIter.next() == edge) {
                throw EdgeSelfReferenceException("Cannot add an edge referencing itself.");
            }
        }
        LayerTagCollection oldTags = getEdgeLayerTags(edge);
        Score oldScore = getEdgeScore(edge);
        if (tags != oldTags) {
            tags = createUnion(oldTags, tags);
        }
        if (edge.implicitIndex < 0) {
            if (tags != oldTags) {
                graph_[edge.descriptor].tagList = tags;
            }
            if (score > oldScore) {
                graph_[edge.descriptor].score = score;
            }
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

        if (tags == getSymbolTag_()) {
            try {
                firstOutEdge(from, getLayerTagManager().getMask(getSymbolTag_()));
                visibleImplicitOutEdges_[from] = true;
                EdgeDescriptor ed = firstInEdge(to, getLayerTagManager().getMask(getSymbolTag_()));
                visibleImplicitOutEdges_[getEdgeSource(ed)] = true;
            } catch (NoEdgeException) {
            }
        }

        if (
            !isLooseVertex(from)
            && !isLooseVertex(to)
            && tags == getSymbolTag_()
            && from + (int) symbolLength_(from) == to
            && !implicitOutEdges_[from]
        ) {
            implicitOutEdges_.set(from, true);
            (insertResult.first)->second = EdgeDescriptor(from);
            return EdgeDescriptor(from);
        }

        Graph::vertex_descriptor boost_from;
        Graph::vertex_descriptor boost_to;

        VerticesMap::iterator iterFrom = vertices_.find(from);
        VerticesMap::iterator iterTo = vertices_.find(to);

        if (iterFrom != vertices_.end()) {
            boost_from = (*iterFrom).second;
        } else {
            boost_from = boost::add_vertex(VertexEntry(from), graph_);
            vertices_[from] = boost_from;
            BOOST_FOREACH(TagCollectionsBimapItem unused, indexedTagCollections_) {
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
            BOOST_FOREACH(TagCollectionsBimapItem unused, indexedTagCollections_) {
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

Lattice::EdgeDescriptor Lattice::addPartitionToEdge(
    EdgeDescriptor edge,
    LayerTagCollection tags,
    EdgeSequence sequence,
    Score score,
    int ruleId) {
    // FIXME - ineffective! addEdge must be refactored
    return addEdge(getEdgeSource(edge),
                   getEdgeTarget(edge),
                   getEdgeAnnotationItem(edge),
                   tags,
                   sequence,
                   score,
                   ruleId);
}

void Lattice::discard(EdgeDescriptor edge) {
    addPartitionToEdge(edge, discardedTag_, EdgeSequence(), (std::numeric_limits<Score>::min)());
}

Lattice::InOutEdgesIterator Lattice::outEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (isLooseVertex(vertex)) {
        Graph::vertex_descriptor boost_vertex = vertices_[vertex];
        if (mask.isAny()) {
            return Lattice::InOutEdgesIterator(
                boost::out_edges(boost_vertex, graph_),
                -1
            );
        }
        int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
        return Lattice::InOutEdgesIterator(
            graph_[boost_vertex].outEdgesIndex[ix].begin(),
            graph_[boost_vertex].outEdgesIndex[ix].end(),
            -1
        );
    } else {
        VerticesMap::iterator iter = vertices_.find(vertex);
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
}

Lattice::InOutEdgesIterator Lattice::inEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (isLooseVertex(vertex)) {
        Graph::vertex_descriptor boost_vertex = vertices_[vertex];
        if (mask.isAny()) {
            return Lattice::InOutEdgesIterator(
                boost::in_edges(boost_vertex, graph_),
                -1
            );
        }
        int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
        return Lattice::InOutEdgesIterator(
            graph_[boost_vertex].inEdgesIndex[ix].begin(),
            graph_[boost_vertex].inEdgesIndex[ix].end(),
            -1
        );
    } else {
        if (vertex == 0) {
            return InOutEdgesIterator();
        }
        VertexDescriptor priorVertex = priorVertex_(vertex);
        VerticesMap::iterator iter = vertices_.find(vertex);
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
    std::string symbol;
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return AnnotationItem(
        "'" + symbol,
        StringFrag(allText_, edge.implicitIndex, symbolLength_(edge.implicitIndex))
    );
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
    return edge.implicitIndex > -1
        && hiddenImplicitOutEdges_[edge.implicitIndex]
        && !visibleImplicitOutEdges_[edge.implicitIndex];
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

const StringFrag Lattice::getEdgeTextAsStringFrag(EdgeDescriptor edge) const {
    if (isLooseVertex(getEdgeSource(edge)) || isLooseVertex(getEdgeTarget(edge))) {
        return StringFrag("@"); // temporarily for edges with loose vertices
    }
    return StringFrag(allText_, getEdgeBeginIndex(edge), getEdgeLength(edge));
}

std::string Lattice::getSequenceText(const EdgeSequence& sequence) {
    std::string r;
    EdgeSequence::Iterator esi(*this, sequence);
    while (esi.hasNext()) {
        r += getEdgeText(esi.next());
    }
    return r;
}

StringFrag Lattice::getSequenceTextAsStringFrag(const EdgeSequence& sequence) {
    StringFrag r;
    EdgeSequence::Iterator esi(*this, sequence);
    while (esi.hasNext()) {
        r.append(getEdgeTextAsStringFrag(esi.next()));
    }
    return r;
}

std::string Lattice::getPartitionText(const Partition& partition) {
    return getSequenceText(partition.getSequence());
}

const std::string Lattice::getAnnotationText(EdgeDescriptor edge) {
    return getEdgeAnnotationItem(edge).getText();
}

const std::string Lattice::getAnnotationCategory(EdgeDescriptor edge) {
    return getEdgeAnnotationItem(edge).getCategory();
}

void Lattice::runCutter(Cutter& cutter, LayerTagMask mask, LayerTagMask superMask) {
    EdgesSortedBySourceIterator edgeIter(*this, superMask);

    while (edgeIter.hasNext()) {
        cutter.reset();
        runCutterOnEdge_(cutter, edgeIter.next(), mask);
    }
}


Lattice::EdgeSequence Lattice::getPath(VertexDescriptor& vertex, LayerTagMask mask) {
    bool nextVertexFound = true;

    Lattice::EdgeSequence::Builder pathBuilder(*this);

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

    } while (nextVertexFound);

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

void Lattice::correctionInsert(VertexDescriptor here, std::string text) {
    VertexDescriptor from = here;
    VertexDescriptor to;
    std::string::iterator iter = text.begin();
    std::string::iterator end = text.end();
    while (iter != end) {
        to = addLooseVertex();
        std::string symbol;
        utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
        addEdge(
            from,
            to,
            AnnotationItem("'"+symbol, StringFrag(symbol)),
            getSymbolTag_()
        );
        from = to;
    }
    try {
        EdgeDescriptor nextEdge
            = firstOutEdge(here, getLayerTagManager().getMask(getSymbolTag_()));
        addEdge(to, getEdgeTarget(nextEdge), getEdgeAnnotationItem(nextEdge), getSymbolTag_());
    } catch (NoEdgeException) {
    }
}

void Lattice::correctionErase(VertexDescriptor from, VertexDescriptor to) {
    if (from != to) {
        try {
            EdgeDescriptor nextEdge
                = firstOutEdge(to, getLayerTagManager().getMask(getSymbolTag_()));
            addEdge(
                from,
                getEdgeTarget(nextEdge),
                getEdgeAnnotationItem(nextEdge),
                getSymbolTag_()
            );
        } catch (NoEdgeException) {
            try {
                EdgeDescriptor prevEdge
                    = firstOutEdge(to, getLayerTagManager().getMask(getSymbolTag_()));
                addEdge(
                    getEdgeSource(prevEdge),
                    to,
                    getEdgeAnnotationItem(prevEdge),
                    getSymbolTag_()
                );
            } catch (NoEdgeException) {
                throw WrongVertexException("Cannot erase the selected text range.");
            }
        }
    }
}

void Lattice::correctionReplace(VertexDescriptor from, VertexDescriptor to, std::string text) {
    try {
        std::string::iterator iter = text.begin();
        std::string::iterator end = text.end();
        while (iter != end) {
            if (from == to) {
                return correctionInsert(from, text);
            }
            std::string symbol;
            utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
            EdgeDescriptor edge
                = firstOutEdge(from, getLayerTagManager().getMask(getSymbolTag_()));
            VertexDescriptor vertex = getEdgeTarget(edge);
            if (symbol != getAnnotationText(edge)) {
                addEdge(
                    from,
                    vertex,
                    AnnotationItem("'"+symbol, StringFrag(symbol)),
                    getSymbolTag_()
                );
            }
            from = vertex;
        }
        correctionErase(from, to);
    } catch (NoEdgeException) {
        throw WrongVertexException("Cannot replace the selected text range.");
    }
}

int Lattice::countEdges(VertexDescriptor from, VertexDescriptor to) {
    return edgeCounterHash_[std::pair<VertexDescriptor, VertexDescriptor>(from, to)];
}

int Lattice::countAllVertices() {
    return 1 + allText_.length() + nLooseVertices_;
}

int Lattice::addTagCollectionIndex_(LayerTagCollection tags) {
    TagCollectionsBimapLeftIterator li = indexedTagCollections_.left.find(tags);
    if (li != indexedTagCollections_.left.end()) {
        return li->second;
    } else {
        int ix = indexedTagCollections_.size();
        BOOST_FOREACH(VerticesMap::value_type iv, vertices_) {
            graph_[iv.second].outEdgesIndex.push_back(
                std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
            );
            BOOST_FOREACH(Graph::edge_descriptor edge, boost::out_edges(iv.second, graph_)) {
                if (createIntersection(graph_[edge].tagList, tags).isNonempty()) {
                    graph_[iv.second].outEdgesIndex[ix].push_back(edge);
                }
            }
            graph_[iv.second].inEdgesIndex.push_back(
                std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
            );
            BOOST_FOREACH(Graph::edge_descriptor edge, boost::in_edges(iv.second, graph_)) {
                if (createIntersection(graph_[edge].tagList, tags).isNonempty()) {
                    graph_[iv.second].inEdgesIndex[ix].push_back(edge);
                }
            }
        }
        indexedTagCollections_.insert(TagCollectionsBimapItem(tags, ix));
        return ix;
    }
}

void Lattice::resizeImplicitEdgesStructures_() {
    implicitOutEdges_.resize(allText_.length()+1);
    hiddenImplicitOutEdges_.resize(allText_.length()+1);
    visibleImplicitOutEdges_.resize(allText_.length()+1);
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

size_t Lattice::symbolLength_(int ix) const {
    return symbolLength(allText_, ix);
}

const LayerTagCollection& Lattice::getSymbolTag_() const {
    return symbolTag_;
}

Lattice::EdgeSequence::EdgeSequence() : begin(0), end(0) {
}

Lattice::EdgeSequence::Iterator::Iterator(
    Lattice & lattice,
    const EdgeSequence & edgeSequence
) :
    lattice_(lattice),
    edgeSequence_(edgeSequence),
    ei_(edgeSequence.links.begin()),
    si_(edgeSequence.begin)
{ }

bool Lattice::EdgeSequence::Iterator::hasNext() {
    if (edgeSequence_.links.empty()) {
        return si_ < edgeSequence_.end;
    } else {
        return ei_ != edgeSequence_.links.end();
    }
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::Iterator::next() {
    if (edgeSequence_.links.empty()) {
        if (si_ >= edgeSequence_.end) {
            throw NoEdgeException("EdgeSequence::Iterator has no next edges.");
        }
        int currentSymbol = si_;
        si_ += lattice_.symbolLength_(si_);
        return lattice_.firstOutEdge(
            lattice_.getVertexForRawCharIndex(currentSymbol),
            lattice_.getLayerTagManager().getMask("symbol")
        );
    } else {
        if (ei_ == edgeSequence_.links.end()) {
            throw NoEdgeException("EdgeSequence::Iterator has no next edges.");
        }
        return *(ei_++);
    }
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::firstEdge(Lattice & lattice) const {
    if (links.empty()) {
        return lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(begin),
            lattice.getLayerTagManager().getMask("symbol")
        );
    } else {
        return links.front();
    }
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::lastEdge(Lattice & lattice) const {
    if (links.empty()) {
        return lattice.firstInEdge(
            lattice.getVertexForRawCharIndex(end),
            lattice.getLayerTagManager().getMask("symbol")
        );
    } else {
        return links.back();
    }
}

bool Lattice::EdgeSequence::empty() const {
    return links.empty() && begin >= end;
}

size_t Lattice::EdgeSequence::size(Lattice & lattice) const {
    if (links.empty()) {
        return utf8::distance(
            lattice.getAllText().begin() + begin,
            lattice.getAllText().begin() + end
        );
    } else {
        return links.size();
    }
}

Lattice::EdgeSequence::Builder& Lattice::EdgeSequence::Builder::addEdge(EdgeDescriptor edge) {
    if (begin <= end && lattice_.isEdgeHidden(edge)) {
        if (links.empty()) {
            begin = lattice_.getEdgeBeginIndex(edge);
            end = lattice_.getEdgeEndIndex(edge);
        } else if (lattice_.getEdgeBeginIndex(edge) == end) {
            end = lattice_.getEdgeEndIndex(edge);
        } else {
            end = 0;
        }
    } else {
        begin = 1;
        end = 0;
    }

    links.push_back(edge);

    return *this;
}

Lattice::EdgeSequence Lattice::EdgeSequence::Builder::build() {
    if (begin < end) {
        return Lattice::EdgeSequence(begin, end);
    } else {
        return Lattice::EdgeSequence(links);
    }
}

Lattice::EdgeSequence::EdgeSequence(const std::vector<EdgeDescriptor>& aLinks) {
    links = aLinks;
    begin = 0;
    end = 0;
}

Lattice::EdgeSequence::EdgeSequence(int aBegin, int aEnd) {
    begin = aBegin;
    end = aEnd;
}


Lattice::Partition::Partition(LayerTagCollection aTagList,
                              EdgeSequence aSequence,
                              Score aScore,
                              int aRuleId):
    sequence_(aSequence), tagList_(aTagList), score_(aScore), ruleId_(aRuleId) {
}


const Lattice::EdgeSequence& Lattice::Partition::getSequence() const {
    return sequence_;
}

const LayerTagCollection& Lattice::Partition::getTagList() const {
    return tagList_;
}

const Lattice::Score& Lattice::Partition::getScore() const {
    return score_;
}

const int& Lattice::Partition::getRuleId() const {
    return ruleId_;
}

Lattice::VertexDescriptor Lattice::firstSequenceVertex_(const EdgeSequence& sequence) {
    return getEdgeSource(sequence.firstEdge(*this));
}

Lattice::VertexDescriptor Lattice::lastSequenceVertex_(const EdgeSequence& sequence) {
    return getEdgeTarget(sequence.lastEdge(*this));
}

void Lattice::runCutterOnEdge_(Cutter& cutter, EdgeDescriptor edge, LayerTagMask mask) {

    VertexDescriptor vertex = getEdgeSource(edge);

    EdgeSequence sequence = getPath(vertex, mask);

    if (sequence.empty())
        return;

    StringFrag text = getSequenceTextAsStringFrag(sequence);

    size_t pos = 0;

    EdgeSequence::Iterator sequenceIter(*this, sequence);

    LayerTagCollection tags = layerTagManager_.createTagCollection(cutter.layerTags());

    do {
        size_t prevPos = pos;

        AnnotationItem item = cutter.cutOff(text, pos);

        int itemLength = (pos == std::string::npos ? text.length() : pos) - prevPos;

        EdgeSequence itemSequence = cutSequenceByTextLength_(sequenceIter, itemLength);

        addEdge(firstSequenceVertex_(itemSequence),
                lastSequenceVertex_(itemSequence),
                item,
                tags,
                itemSequence);

    } while (pos < text.length() && pos != std::string::npos);
}

Lattice::EdgeSequence Lattice::cutSequenceByTextLength_(
    EdgeSequence::Iterator& sequenceIterator,
    int length
) {
    int lengthGathered = 0;

    EdgeSequence::Builder sequenceBuilder(*this);

    while (sequenceIterator.hasNext() && lengthGathered < length) {
        EdgeDescriptor ed = sequenceIterator.next();
        sequenceBuilder.addEdge(ed);
        lengthGathered += getEdgeText(ed).length();
    }

    return sequenceBuilder.build();
}

inline bool compareSecond_(
    std::pair<int, int> p1,
    std::pair<int, int> p2
) {
    return p1.second < p2.second;
}

Lattice::VertexIterator::VertexIterator(Lattice& lattice) :
    lattice_(lattice),
    vd_(0),
    withLooseVertices_(lattice.nLooseVertices_ > 0)
{
    if (lattice.nLooseVertices_ > 0) {
        std::list<Graph::vertex_descriptor> vertexContainer;
        boost::topological_sort(lattice_.graph_, std::front_inserter(vertexContainer));
        BOOST_FOREACH(Graph::vertex_descriptor vd, vertexContainer) {
            iterContainer_.push_back(
                std::pair<Lattice::VertexDescriptor, int>(lattice.graph_[vd].index, f_(vd))
            );
        }
        VertexDescriptor vd = 0;
        while (vd < int(lattice.allText_.length())) {
            if (lattice.vertices_.find(vd) == lattice.vertices_.end()) {
                iterContainer_.push_back(std::pair<Lattice::VertexDescriptor, int>(vd, vd));
            }
            vd += lattice_.symbolLength_((int)vd);
        }
        if (lattice.vertices_.find(vd) == lattice.vertices_.end()) {
            iterContainer_.push_back(std::pair<Lattice::VertexDescriptor, int>(vd, vd));
        }
        std::stable_sort(iterContainer_.begin(), iterContainer_.end(), compareSecond_);
        ici_ = iterContainer_.begin();
    }
}

bool Lattice::VertexIterator::hasNext() {
    if (withLooseVertices_) {
        return ici_ != iterContainer_.end();
    }
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
    if (withLooseVertices_) {
        if (ici_ != iterContainer_.end()) return (*(ici_++)).first;
    }
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

int Lattice::VertexIterator::f_(Graph::vertex_descriptor vd) {
    int index = lattice_.graph_[vd].index;
    if (index > -1) {
        return index;
    }
    int maxIndex = -1;
    BOOST_FOREACH(Graph::edge_descriptor edge, boost::in_edges(vd, lattice_.graph_)) {
        index = f_(boost::source(edge, lattice_.graph_));
        if (index > maxIndex) {
            maxIndex = index;
        }
    }
    return maxIndex;
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

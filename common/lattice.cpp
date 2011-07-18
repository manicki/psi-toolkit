#include "lattice.hpp"

Lattice::Lattice(std::string text) : layerTagManager_() {
    Lattice::VertexDescriptor vertex = boost::add_vertex(graph_);
    vertices_.push_back(vertex);
    appendString(text);
}

Lattice::~Lattice() {
    //TODO
}

void Lattice::appendString(std::string text) {
    UTF8String utext(text);
    for (UTF8String::iterator uti = utext.begin(); uti != utext.end(); ++uti) {
        
        Lattice::VertexDescriptor vertex = boost::add_vertex(graph_);
        vertices_.push_back(vertex);
        
        for (int j = 0; j < indexedTagCollections_.size(); ++j) {
            graph_[vertex].outEdgesIndex.push_back(std::list<EdgeDescriptor>());
            graph_[vertex].inEdgesIndex.push_back(std::list<EdgeDescriptor>());
        }
        
        addEdge(
            vertices_.back(),
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
        AnnotationItem
    > hkey(vpair, annotationItem);
    
    std::pair<VVCHash::iterator, bool> insertResult(vvcHash_.insert(
        std::pair<
            std::pair<std::pair<VertexDescriptor, VertexDescriptor>, AnnotationItem>,
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

std::pair<Lattice::EdgeDescriptorIterator, Lattice::EdgeDescriptorIterator> Lattice::outEdges(
    Lattice::VertexDescriptor vertex, 
    LayerTagMask mask
) {
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return std::pair<Lattice::EdgeDescriptorIterator, Lattice::EdgeDescriptorIterator>(
        graph_[vertex].outEdgesIndex[ix].begin(),
        graph_[vertex].outEdgesIndex[ix].end()
    );
}

std::pair<Lattice::EdgeDescriptorIterator, Lattice::EdgeDescriptorIterator> Lattice::inEdges(
    Lattice::VertexDescriptor vertex, 
    LayerTagMask mask
) {
    int ix = addTagCollectionIndex_(layerTagManager_.createTagCollection(mask));
    return std::pair<Lattice::EdgeDescriptorIterator, Lattice::EdgeDescriptorIterator>(
        graph_[vertex].inEdgesIndex[ix].begin(),
        graph_[vertex].inEdgesIndex[ix].end()
    );
}

std::pair<Lattice::OutEdgeIterator, Lattice::OutEdgeIterator> Lattice::allOutEdges(
    Lattice::VertexDescriptor vertex
) {
    return boost::out_edges(vertex, graph_);
}

std::pair<Lattice::InEdgeIterator, Lattice::InEdgeIterator> Lattice::allInEdges(
    Lattice::VertexDescriptor vertex
) {
    return boost::in_edges(vertex, graph_);
}

Lattice::EdgeDescriptor Lattice::firstOutEdge(
    Lattice::VertexDescriptor vertex, 
    LayerTagMask mask
) {
    if (mask.isAny()) return *(boost::out_edges(vertex, graph_).first);
    return *(outEdges(vertex, mask).first);
}

Lattice::EdgeDescriptor Lattice::firstInEdge(
    Lattice::VertexDescriptor vertex, 
    LayerTagMask mask
) {
    if (mask.isAny()) return *(boost::in_edges(vertex, graph_).first);
    return *(inEdges(vertex, mask).first);
}

std::list<Lattice::EdgeDescriptor> Lattice::edgesSorted(LayerTagMask mask) {
    std::list<Lattice::EdgeDescriptor> result;
    for (
        std::vector<Lattice::VertexDescriptor>::iterator vi = vertices_.begin();
        vi != vertices_.end();
        ++vi
    ) {
        std::pair<Lattice::EdgeDescriptorIterator,Lattice::EdgeDescriptorIterator> 
            outEdgesIters = outEdges(*vi, mask);
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

#ifndef LATTICE_HDR
#define LATTICE_HDR


#include <string>
#include <list>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include "layer_tag_manager.hpp"
#include "annotation_item.hpp"
#include "exceptions.hpp"

/*!
  Lattice is used to keep all the information extracted by annotators
  (processors). Language units (tokens, words, phrases etc.)
  recognised by annotators are represented by edges (whereas vertices
  represent points between characters). Each edge is assigned an annotation
  item which is composed of a category and a attribute-value matrix (AVM).
  Each edge has tags which can be used to distinguish various levels, annotators etc.

  Lattice lifecycle:
  - construction, edge is created for each character (optimised!) [tag: raw]
  - formatter, extra edges for mark-ups and entity-likes (one edge "<"
    spanning "&lt;"), [text, mark-ups]
  - splitter - working on text level, Translatica splitter with
    function, get_text (....) [sentence]
  - tokeniser - working on text level (inside sentence?), get_edge_text [token]
  - lemmatiser - working on token level [lemma]
  - corrector - working on text level (if token without lemma),
  - parser - working ok token/lemma level, taking into account blanks
  - chooser - working on parser level
*/


class Lattice {
    
public:

    struct VertexEntry {
        //TODO
    };
    
    struct EdgeEntry {
        AnnotationItem category;
        LayerTagCollection tagList;
        
        EdgeEntry(
            AnnotationItem aCategory, 
            LayerTagCollection aTagList
        ): category(aCategory), tagList(aTagList) { }
    };

    typedef boost::adjacency_list<
        boost::vecS, 
        boost::vecS, 
        boost::bidirectionalS,
        VertexEntry, 
        EdgeEntry
    > Graph;

    typedef Graph::vertex_descriptor VertexDescriptor;
    typedef Graph::edge_descriptor EdgeDescriptor;
    typedef Graph::out_edge_iterator OutEdgeIterator;
    typedef Graph::in_edge_iterator InEdgeIterator;

public:

    /**
     * Creates a lattice from `text`. Initially each character of text will be
     * represented as an edge labeled with layer tag `raw` and category `c`
     * (where c is the given character)
     */
    Lattice(std::string text);

    ~Lattice();

    /**
     * Gets the vertex for ix-th character of text
     */
    VertexDescriptor getVertexForRawCharIndex(int ix);

    /**
     * Gets the first vertex (the same as getVertexForRawCharIndex(0))
     */
    VertexDescriptor getFirstVertex();

     /**
     * Gets the last vertex
     */
    VertexDescriptor getLastVertex();

    /**
     * Adds an edge from vertex `from` to vertex `to` with `annonation_item`
     * and `tags` as layer tags. The partition of the edge into subedges 
     * is given with `partition` argument.
     *
     * A new will not be added if there is already an edge from vertex `from`
     * to vertex `to` of annotation `annotation_item` (tags and partitions will
     * be updated in such a case).
     */
    EdgeDescriptor addEdge(VertexDescriptor from,
                           VertexDescriptor to,
                           const AnnotationItem& annotationItem,
                           LayerTagCollection tags,
                           std::list<EdgeDescriptor> partition);

    // return outgoing edges which has at least one layer tag from `mask`
    std::pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor vertex, LayerTagCollection mask) const;

    std::pair<InEdgeIterator, InEdgeIterator> inEdges(VertexDescriptor vertex, LayerTagCollection mask) const;

    EdgeDescriptor firstOutEdge(VertexDescriptor vertex, LayerTagCollection mask) const;
    EdgeDescriptor firstInEdge(VertexDescriptor vertex, LayerTagCollection mask) const;


    // returns the list of edges which have at least one layer tag from `mask` sorted
    std::list<EdgeDescriptor> edgesSorted(LayerTagCollection mask);

    LayerTagManager& getLayerTagManager();

    const AnnotationItem& getEdgeAnnotationItem(EdgeDescriptor edge);
    const LayerTagCollection& getEdgeLayerTags(EdgeDescriptor edge);

private:
    
    Graph graph_;
    
    LayerTagManager layerTagManager_;
    
    /**
     * Maintains the topologically ordered vertices.
     */
    std::vector<VertexDescriptor> vertices_;

};


#endif


#ifndef LATTICE_HDR
#define LATTICE_HDR


#include <string>
#include <list>

#include <boost/graph/adjacency_list.hpp>

#include "layer_tag_manager.hpp"
#include "annotation_item.hpp"

/*!
  Lattice is used to keep all the information extracted by annotators
  (processors). Language units (tokens, words, phrases etc.)
  recognised by annotators are represented by edges (vertices
  represent points between characters). Each edge has a category and
  a attribute-value matrix (AVM). Each edge has tags which can
  be used to distinguish various levels, annotators etc.

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
    };

    typedef boost::adjacency_list<
        boost::vecS, 
        boost::vecS, 
        boost::bidirectionalS,
        VertexEntry, 
        EdgeEntry
    >::vertex_descriptor VertexDescriptor;
    
    typedef boost::adjacency_list<
        boost::vecS, 
        boost::vecS, 
        boost::bidirectionalS,
        VertexEntry, 
        EdgeEntry
    >::edge_descriptor EdgeDescriptor;
    
    typedef boost::adjacency_list<
        boost::vecS, 
        boost::vecS, 
        boost::bidirectionalS,
        VertexEntry, 
        EdgeEntry
    >::out_edge_iterator OutEdgeIterator;
    
    typedef boost::adjacency_list<
        boost::vecS, 
        boost::vecS, 
        boost::bidirectionalS,
        VertexEntry, 
        EdgeEntry
    >::in_edge_iterator InEdgeIterator;

    /**
     * starts with text, for each character of `text` edge of layer tag `raw` and
     * category `'c` (where c is a characted should be added
     */
    Lattice(std::string text);
    ~Lattice();

    /**
     * get vertex for ix-th character of text
     */
    VertexDescriptor getVertexForRawCharIndex(int ix);

    // the same as getVertexForRawCharIndex(0)
    VertexDescriptor getFirstVertex();

    VertexDescriptor getLastVertex();

    EdgeDescriptor addEdge(VertexDescriptor from,
                           VertexDescriptor to,
                           const AnnotationItem& annotation_item,
                           LayerTagCollection tags,
                           std::list<EdgeDescriptor> partition);

    // return outgoing edges which has at least one layer tag from `mask`
    std::pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor vertex, LayerTagCollection mask) const;

    std::pair<InEdgeIterator, InEdgeIterator> inEdges(VertexDescriptor vertex, LayerTagCollection mask) const;

    EdgeDescriptor firstOutEdge(VertexDescriptor vertex, LayerTagCollection mask) const;
    EdgeDescriptor firstInEdge(VertexDescriptor vertex, LayerTagCollection mask) const;


    // returns the list of edges which have at least one layer tag from `mask` sorted topologically
    std::list<EdgeDescriptor> edgesSortedTopologically(LayerTagCollection mask);

    LayerTagManager& getLayerTagManager();

    const AnnotationItem& getEdgeAnnotationItem(EdgeDescriptor edge);
    const LayerTagCollection& getEdgeLayerTags(EdgeDescriptor edge);

};


#endif


#ifndef LATTICE_HDR
#define LATTICE_HDR


#include <string>
#include <list>
#include <pair>

#include "layer_tag_collection.hpp"
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

    typedef .... VertexDescriptor;
    typedef .... EdgeDescriptor;
    typedef .... OutEdgeIterator;
    typedef .... InEdgeIterator;

public:
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
                           List<EdgeDescriptor> partition);

    // return outgoing edges which has at least one layer tag from `mask`
    std::pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor vertex, LayerTagCollection mask) const;

    std::pair<InEdgeIterator, InEdgeIterator> inEdges(VertexDescriptor vertex, LayerTagCollecion mask) const;

    EdgeDescriptor firstOutEdge(VertexDescriptor vertex, LayerTagCollecion mask) const;
    EdgeDescriptor firstInEdge(VertexDescriptor vertex, LayerTagCollecion mask) const;


    // returns the list of edges which have at least one layer tag from `mask` sorted topologically
    std::list<EdgeDescriptor> edgesSortedTopologically(LayerTagCollection mask);

    LayerTagManager& getLayerTagManager();

    const AnnotationItem& getEdgeAnnotationItem(EdgeDescriptor edge);
    const LayerTagCollection& getEdgeLayerTags(EdgeDescriptor edge);

}


#endif


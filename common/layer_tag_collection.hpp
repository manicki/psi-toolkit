#ifndef LAYER_TAG_COLLECTION_HDR
#define LAYER_TAG_COLLECTION_HDR

#include <boost/dynamic_bitset.hpp>

/*!
  A large number of various annotations originated from various
  annotators (eg. tokeniser, parser, translator etc.) will be kept. In
  order to distinguish annotations of different origins, layer tags
  are introduced. Each lattice edge can be tagged with multiple layer tags.
  Layer tags are flexible, the following things can be expressed with layer tags:
  * annotation level (token, segment, phrase etc.)
  * annotator used (gobio, puddle, translatica-tokenizer etc.)
  * tagset used (translatica-dic, morfeusz, kipi etc.)

  To keep layer tags, LayerTagCollection are introduced (there is no
  LayerTag class).

  LayerTagCollection can be created with LayerTagManager.

  */
class LayerTagCollection {

public:
    bool isEmpty();

    bool isNonempty();

    /**
     * method for compatibility with boost::bimap
     */
    bool operator<(LayerTagCollection other) const;

    bool operator==(const LayerTagCollection& other) const;

    bool operator!=(const LayerTagCollection& other) const;

    /**
     * method for compatibility with HashWrapper3
     */
    unsigned long getHash() const;

    /**
     * created union of two layertaglists
     */
    friend LayerTagCollection createUnion(
        LayerTagCollection tag_list_a,
        LayerTagCollection tag_list_b
    );

    /**
     * created intersection of two layertaglists
     */
    friend LayerTagCollection createIntersection(
        LayerTagCollection tag_list_a,
        LayerTagCollection tag_list_b
    );

    friend class LayerTagManager;

    friend class LayerTagMask;

private:
    /**
     * private constructor! only LayerTagManager can be used to create layer tag collecions
     */
    LayerTagCollection() : v_(32) {};

    /**
     * private constructor! only LayerTagManager can be used to create layer tag collecions
     */
    LayerTagCollection(int size) : v_(size % 32 == 0 ? size : size + 32 - size % 32) { };

    boost::dynamic_bitset<> v_;

    /**
     * resizes to the nearest power of two
     */
    boost::dynamic_bitset<>::size_type resize_(boost::dynamic_bitset<>::size_type size);

    /**
     * equalizes lengths of two LayerTagCollection vectors
     */
    boost::dynamic_bitset<>::size_type resize_(LayerTagCollection& other);

};


#endif

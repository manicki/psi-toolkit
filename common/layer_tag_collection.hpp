#ifndef LAYER_TAG_COLLECTION_HDR
#define LAYER_TAG_COLLECTION_HDR

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
    bool isEmpty() { /*TODO*/ };

    /**
     * created union of two layertaglists
     */
    friend LayerTagCollection createUnion(LayerTagCollection tag_list_a, LayerTagCollection tag_list_b) { /*TODO*/ };

    /**
     * created intersection of two layertaglists
     */
    friend LayerTagCollection createIntersection(LayerTagCollection tag_list_a, LayerTagCollection tag_list_b);

    friend class LayerTagManager;

private:
    /**
     * private constructor! only LayerTagManager can be used to create layer tag collecions
     */
    LayerTagCollection();

};


#endif
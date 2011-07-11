#include "layer_tag_collection.hpp"

bool LayerTagCollection::isEmpty() {
    return false;
}

LayerTagCollection createUnion(LayerTagCollection tag_list_a, LayerTagCollection tag_list_b) {
    tag_list_a.v_ |= tag_list_b.v_;
    return tag_list_a;
}

LayerTagCollection createIntersection(LayerTagCollection tag_list_a, LayerTagCollection tag_list_b) {
    tag_list_a.v_ &= tag_list_b.v_;
    return tag_list_a;
}

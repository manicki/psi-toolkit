#include "layer_tag_collection.hpp"

bool LayerTagCollection::isEmpty() {
    return v_.none();
}

bool LayerTagCollection::isNonempty() {
    return v_.any();
}

bool LayerTagCollection::operator<(LayerTagCollection other) const {
    return v_ < other.v_;
}

int LayerTagCollection::resize_(int newSize) {
    int size = v_.size();
    if (newSize > size) {
        do {
            size *= 2;
        } while (newSize > size);
        v_.resize(size);
    }
    return size;
}

int LayerTagCollection::resize_(LayerTagCollection other) {
    if (v_.size() < other.v_.size()) {
        return resize_(other.v_.size());
    } else if (v_.size() > other.v_.size()) {
        return other.resize_(v_.size());
    }
    return v_.size();
}

LayerTagCollection createUnion(LayerTagCollection tag_list_a, LayerTagCollection tag_list_b) {
    LayerTagCollection result(tag_list_a.resize_(tag_list_b));
    result.v_ |= tag_list_a.v_;
    result.v_ |= tag_list_b.v_;
    return result;
}

LayerTagCollection createIntersection(LayerTagCollection tag_list_a, LayerTagCollection tag_list_b) {
    LayerTagCollection result(tag_list_a.resize_(tag_list_b));
    result.v_ |= tag_list_a.v_;
    result.v_ &= tag_list_b.v_;
    return result;
}

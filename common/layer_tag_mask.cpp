#include "layer_tag_mask.hpp"

bool LayerTagMask::isNone() {
    return none_ || (tags_->isEmpty() && (none_ = true));
}

bool LayerTagMask::isSome() {
    return any_ || tags_->isNonempty();
}

bool LayerTagMask::isAny() {
    return any_;
}

bool LayerTagMask::operator<(LayerTagMask other) const {
    return *tags_ < *(other.tags_);
}

LayerTagMask createUnion(
    LayerTagMask mask1, 
    LayerTagMask mask2
) {
    if (mask1.any_ || mask2.any_) return LayerTagMask(true);
    if (mask1.none_) return mask2;
    if (mask2.none_) return mask1;
    LayerTagMask result;
    result.tags_ = new LayerTagCollection(createUnion(*(mask1.tags_), *(mask2.tags_)));
    return result;
}

LayerTagMask createIntersection(
    LayerTagMask mask1, 
    LayerTagMask mask2
) {
    if (mask1.none_ || mask2.none_) return LayerTagMask(false);
    if (mask1.any_) return mask2;
    if (mask2.any_) return mask1;
    LayerTagMask result;
    result.tags_ = new LayerTagCollection(createIntersection(*(mask1.tags_), *(mask2.tags_)));
    return result;
}

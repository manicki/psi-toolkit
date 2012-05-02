#include "layer_tag_mask.hpp"

bool LayerTagMask::isNone() {
    return none_ || (!any_ && !plane_ && tags_.isEmpty() && (none_ = true));
}

bool LayerTagMask::isSome() {
    return any_ || tags_.isNonempty();
}

bool LayerTagMask::isAny() const {
    return any_;
}

bool LayerTagMask::isPlane() const {
    return plane_;
}

bool LayerTagMask::operator<(LayerTagMask other) const {
    return tags_ < other.tags_;
}

LayerTagMask createUnion(
    LayerTagMask mask1,
    LayerTagMask mask2
) {
    if (mask1.any_ || mask2.any_) return LayerTagMask(true);
    if (mask1.none_) return mask2;
    if (mask2.none_) return mask1;
    assert(!mask1.plane_);
    assert(!mask2.plane_);
    return LayerTagMask(createUnion(mask1.tags_, mask2.tags_));
}

LayerTagMask createIntersection(
    LayerTagMask mask1,
    LayerTagMask mask2
) {
    if (mask1.none_ || mask2.none_) return LayerTagMask(false);
    if (mask1.any_) return mask2;
    if (mask2.any_) return mask1;
    assert(!mask1.plane_);
    assert(!mask2.plane_);
    return LayerTagMask(createIntersection(mask1.tags_, mask2.tags_));
}

bool matches(
    LayerTagCollection tags,
    LayerTagMask mask) {

    if (mask.none_)
        return false;

    if (mask.any_)
        return true;

    return createIntersection(mask.tags_, tags).isNonempty();
}


#include "layer_tag_mask.hpp"

bool LayerTagMask::isNone() {
    return none_ || (!any_ && tagAlts_[0].isEmpty() && (none_ = true));
}

bool LayerTagMask::isSome() {
    return any_ || tagAlts_[0].isNonempty();
}

bool LayerTagMask::isAny() const {
    return any_;
}

bool LayerTagMask::operator<(LayerTagMask other) const {
    return tagAlts_[0] < other.tagAlts_[0];
}

bool matches(
    LayerTagCollection tags,
    LayerTagMask mask) {

    if (mask.none_)
        return false;

    if (mask.any_)
        return true;

    return createIntersection(mask.tagAlts_[0], tags).isNonempty();
}


#include "layer_tag_mask.hpp"

#include <boost/foreach.hpp>

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

    BOOST_FOREACH(LayerTagCollection tagAlt, mask.tagAlts_) {
        if (createIntersection(tagAlt, tags).isNonempty())
            return true;
    }

    return false;
}


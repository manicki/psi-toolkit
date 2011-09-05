#include "segment_glob.hpp"

#include "utf8.h"

SegmentGlob::SegmentGlob(const std::string& glob):glob_(glob) {
    checkIfFixed_();
}

bool SegmentGlob::matches(const std::string& segment) const {

    if (isFixed_)
        return segment == glob_;

    const char* cp;
    const char* mp;

    const char* s = segment.c_str();
    const char* g = glob_.c_str();

    while ((*s) && (*g != '*')) {
        if ((*g != *s) && (*g != '?')) {
            return false;
        }
        if (*g == '?')
            utf8::unchecked::next(s);
        else
            ++s;

        ++g;
    }

    while (*s) {
        if (*g == '*') {
            if (!*++g) {
                return true;
            }
            mp = g;
            cp = s+1;
        } else if (*g == '?') {
            ++g;
            utf8::unchecked::next(s);
        } else if (*g == *s) {
            ++g;
            ++s;
        } else {
            g = mp;
            s = cp++;
        }
    }

    while (*g == '*')
        ++g;

    return !*g;
}

bool SegmentGlob::isFixed() const {
    return isFixed_;
}

const std::string& SegmentGlob::asString() const {
    return glob_;
}

void SegmentGlob::checkIfFixed_() {
    isFixed_ =
        (glob_.find_first_of("*?") == std::string::npos);
}

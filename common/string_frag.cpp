#include "string_frag.hpp"


StringFrag & StringFrag::operator=(const StringFrag & other) {
    if (!valid()) {
        throw StringFragException("String frag invalidated (" + contents_ + "...)");
    }
    if (!other.valid()) {
        throw StringFragException("String frag invalidated (" + other.contents_ + "...)");
    }
    if (this != &other) {
        this->StringFrag::~StringFrag();
        new (this) StringFrag(other);
    }
    return *this;
}

const char & StringFrag::operator[](size_t pos) const {
    if (!valid()) {
        throw StringFragException("String frag invalidated (" + contents_ + "...)");
    }
    if (begin_ == std::string::npos || len_ == std::string::npos) {
        return contents_[pos];
    }
    return src_[begin_ + pos];
}

std::string StringFrag::str() const {
    if (!valid()) {
        throw StringFragException("String frag invalidated (" + contents_ + "...)");
    }
    if (begin_ == std::string::npos || len_ == std::string::npos) {
        return contents_;
    }
    return src_.substr(begin_, len_);
}

std::string StringFrag::substr(size_t pos, size_t n) const {
    return str().substr(pos, n);
}

void StringFrag::append(const StringFrag & other) {
    if (!valid()) {
        throw StringFragException("String frag invalidated (" + contents_ + "...)");
    }
    if (!other.valid()) {
        throw StringFragException("String frag invalidated (" + other.contents_ + "...)");
    }
    if (
        begin_ != std::string::npos &&
        other.begin_ != std::string::npos &&
        len_ != std::string::npos &&
        other.len_ != std::string::npos &&
        src_ == other.src_ &&
        begin_ + len_ == other.begin_
    ) {
        len_ += other.len_;
        contents_ = src_.substr(begin_, std::min(len_,(size_t)4));
    } else if (contents_=="" && this != &other) {
        this->StringFrag::~StringFrag();
        new (this) StringFrag(other);
    } else {
        contents_ = str() + other.str();
        begin_ = std::string::npos;
        len_ = std::string::npos;
    }
}

size_t StringFrag::find(char c, size_t pos) const {
    if (!valid()) {
        throw StringFragException("String frag invalidated (" + contents_ + "...)");
    }
    if (begin_ == std::string::npos) {
        return contents_.find(c, pos);
    }
    return src_.find(c, begin_ + pos) - begin_;
}

size_t StringFrag::length() const {
    if (begin_ == std::string::npos) {
        return contents_.length();
    }
    return len_;
}

bool StringFrag::valid() const {
    if (
        begin_ == std::string::npos
        || len_ == std::string::npos
        || (begin_ + len_ <= src_.length()
            && src_.substr(begin_, (len_ < 4 ? len_ : 4)) == contents_)
    ) return true;
    return false;
}

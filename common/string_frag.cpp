#include "string_frag.hpp"


StringFrag & StringFrag::operator=(const StringFrag & other) {
    if (this != &other) {
        this->StringFrag::~StringFrag();
        new (this) StringFrag(other);
    }
    return *this;
}

std::string StringFrag::str() const {
    if (begin_ == std::string::npos) {
        return contents_;
    }
    return src_.substr(begin_, len_);
}

void StringFrag::append(const StringFrag & other) {
    if (
        begin_ != std::string::npos &&
        other.begin_ != std::string::npos &&
        len_ != std::string::npos &&
        other.len_ != std::string::npos &&
        src_ == other.src_ &&
        begin_ + len_ == other.begin_
    ) {
        len_ += other.len_;
    } else {
        contents_ = str() + other.str();
        begin_ = std::string::npos;
        len_ = std::string::npos;
    }
}

size_t StringFrag::find(char c, size_t pos) const {
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

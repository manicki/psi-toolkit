#include <string>


class StringFrag {
public:
    StringFrag() :
        contents_(""),
        src_(contents_),
        begin_(std::string::npos),
        len_(std::string::npos)
    { }

    StringFrag(const StringFrag & other) :
        contents_(other.contents_),
        src_(other.src_),
        begin_(other.begin_),
        len_(other.len_)
    { }

    StringFrag(
        const std::string & src,
        size_t begin,
        size_t len
    ) :
        contents_(""),
        src_(src),
        begin_(begin),
        len_(len)
    { }

    StringFrag(
        std::string contents
    ) :
        contents_(contents),
        src_(contents_),
        begin_(std::string::npos),
        len_(std::string::npos)
    { }

    StringFrag & operator=(const StringFrag & other) {
        if (this != &other) {
            this->StringFrag::~StringFrag();
            new (this) StringFrag(other);
        }
        return *this;
    }

    std::string str() const {
        if (begin_ == std::string::npos) {
            return contents_;
        }
        return src_.substr(begin_, len_);
    }

    void append(const StringFrag & other) {
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

private:
    std::string contents_;
    const std::string & src_;
    size_t begin_;
    size_t len_;
};

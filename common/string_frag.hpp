#include <string>


class StringFrag {
public:
    StringFrag() :
        contents_(""),
        src_(contents_),
        begin_(0),
        len_(0)
    { }

    StringFrag(
        std::string & src,
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

    std::string str() const {
        // if (begin_ != std::string::npos) {
            // return src_.substr(begin_, len_);
        // }
        return contents_;
    }

private:
    std::string contents_;
    std::string & src_;
    size_t begin_;
    size_t len_;
};

#ifndef STRING_FRAG_HDR
#define STRING_FRAG_HDR


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
        const StringFrag & sf,
        size_t begin,
        size_t len
    ) :
        contents_(""),
        src_(sf.src_),
        begin_(sf.begin_ + begin),
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

    StringFrag & operator=(const StringFrag & other);

    std::string str() const;

    void append(const StringFrag & other);

    size_t find(char c, size_t pos = 0) const;

private:
    std::string contents_;
    const std::string & src_;
    size_t begin_;
    size_t len_;
};


#endif

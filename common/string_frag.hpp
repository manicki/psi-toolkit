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
        unsigned short int begin,
        unsigned short int len
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
        begin_(0),
        len_((unsigned short int) contents.length())
    { }

    std::string str() const {
        if (contents_.empty()) return src_.substr(begin_, len_);
        return contents_;
    }

private:
    std::string contents_;
    std::string & src_;
    unsigned short int begin_;
    unsigned short int len_;
};

#include <string>


class StringFrag {
public:
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
        std::string & contents
    ) :
        contents_(contents),
        begin_(0),
        len_(0)
    { }

    std::string str() {
        if (contents_.empty()) return src_.substr(begin_, len_);
        return contents_;
    }

private:
    std::string contents_;
    std::string & src_;
    unsigned short int begin_;
    unsigned short int len_;
};

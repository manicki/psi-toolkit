#include <string>


class Substring {
public:
    Substring(
        const std::string source,
        char begin,
        char len
    ) :
        source_(source),
        begin_(begin),
        len_(len)
    { }

    std::string toString() {
        return source_.substr(begin_, len_);
    }

private:
    std::string source_;
    unsigned short int begin_;
    unsigned short int len_;
};

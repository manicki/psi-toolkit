#ifndef STRING_REPRESENTATIONER_HPP_HDR
#define STRING_REPRESENTATIONER_HPP_HDR

#include <string>

template<class T>
class string_representationer
{
public:
    virtual std::string string_representation(T val) = 0;

    virtual ~string_representationer() {}
};

template<class T, class M>
class master_string_representationer: public string_representationer<T>
{
private:
    M& master_;

public:
    master_string_representationer(M& master): master_(master) { };
    ~master_string_representationer() {}

    std::string string_representation(T val)
    {
        return master_.string_representation(val);
    }
};


#endif

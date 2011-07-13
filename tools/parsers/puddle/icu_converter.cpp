#include "icu_converter.hpp"

#include <iostream>
#include <fstream>

UnicodeString icuConverter::fromUtf(std::string utfString)
{
    int uniSize = utfString.length();
//    UnicodeString result(utfString.c_str(), utfString.length(), "UTF-8");
    UnicodeString result(utfString.c_str(), uniSize);
//    UnicodeString us = result;
//    us.toUpper();
//    char *buf= new char[us.length() * 4];
//    us.extract(0, us.length(), buf, us.length() * 4, "UTF-8");
//    std::string n(buf);
//    std::cout << "uc: " << n << std::endl;
//    delete buf;
//    us.toLower();
//    buf= new char[us.length() * 4];
//    us.extract(0, us.length(), buf, us.length() * 4, "UTF-8");
//    n = buf;
//    std::cout << "lc: " << n << std::endl;
//    delete buf;
    return result;
}

std::string icuConverter::toUtf(UnicodeString uniString)
{
    int utfSize = 3 * uniString.length() + 1;
    char *buffer = new char[utfSize];
    uniString.extract(0, uniString.length(), buffer, utfSize, "UTF-8");
    std::string result(buffer);
    delete[] buffer;
//    std::cout << "wyjdzie z przemiany: " << result << std::endl;
    return result;
}

//int main(int argc, char **argv)
//{
//    std::ifstream ifstr("znaki.txt");
//    while (!ifstr.eof())
//    {
//        std::string s;
//        getline(ifstr, s);
//        std::cout << "stl: " << s << std::endl;
//        UnicodeString us = IcuConverter::fromUtf(s);
//        us.toUpper();
//        std::cout << "uc:  " << IcuConverter::toUtf(us) << std::endl;
//        us.toLower();
//        std::cout << "lc:  " << IcuConverter::toUtf(us) << std::endl;
//    }
//    return 0;
//}

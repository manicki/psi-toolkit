#ifndef UTF8CONVERTER_H__
#define UTF8CONVERTER_H__

#include <string>

namespace utf8converter
{
//class Utf8Converter
//{
//    public:
        std::string ToUtf8(const std::wstring& widestring);
        std::wstring FromUtf8(const std::string& utf8string);
//};
}

#endif

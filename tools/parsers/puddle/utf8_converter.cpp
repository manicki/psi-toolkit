#include <iostream>
#include <locale>
#include <fstream>
#include <cstring>
//#include <stdint.h>

#include "utf8_converter.hpp"

#include "convert_utf.h"

namespace utf8converter {
    //std::wstring Utf8Converter::FromUtf8(const std::string& utf8string)
    std::wstring FromUtf8(const std::string& utf8string)
    {
        size_t widesize = utf8string.length();
        if (sizeof(wchar_t) == 2)
        {
            wchar_t* wstringnat = new wchar_t[widesize + 1];
            memset(wstringnat, L'\0', widesize);
            //std::wstring resultString;
            //resultString.resize(widesize+1, L'\0');
            const UTF8* sourceStart = reinterpret_cast<const UTF8*>(utf8string.c_str());
            const UTF8* sourceEnd = sourceStart + widesize;
            //UTF16* targetStart = reinterpret_cast<UTF16*>(&resultString[0]);
            UTF16* targetStart = reinterpret_cast<UTF16*>(wstringnat);
            UTF16* targetEnd = targetStart + widesize;
            ConversionResult res = ConvertUTF8toUTF16(&sourceStart, sourceEnd, &targetStart, targetEnd, strictConversion);
            if (res != conversionOK)
            {
                delete [] wstringnat;
                std::cerr << "Blad konwersji" << std::endl;
                //throw std::exception("La falla!");
            }
            *targetStart = 0;
            std::wstring resultString(wstringnat);
            delete [] wstringnat;
            return resultString;
        }
        else if (sizeof(wchar_t) == 4)
        {
            wchar_t* wstringnat = new wchar_t[widesize + 1];
            memset(wstringnat, L'\0', widesize);
            //std::wstring resultString;
            //resultString.resize(widesize+1, L'\0');
            const UTF8* sourceStart = reinterpret_cast<const UTF8*>(utf8string.c_str());
            const UTF8* sourceEnd = sourceStart + widesize;
            //UTF32* targetStart = reinterpret_cast<UTF32*>(&resultString[0]);
            UTF32* targetStart = reinterpret_cast<UTF32*>(wstringnat);
            UTF32* targetEnd = targetStart + widesize;
            ConversionResult res = ConvertUTF8toUTF32(&sourceStart, sourceEnd, &targetStart, targetEnd, strictConversion);
            if (res != conversionOK)
            {
                delete [] wstringnat;
                std::cerr << "Blad konwersji1: " << utf8string << std::endl;
                //throw std::exception("La falla!");
            }
            *targetStart = 0;
            std::wstring resultString(wstringnat);
            delete [] wstringnat;
            return resultString;
        }
        else
        {
            std::cerr << "Blad konwersji2!" << std::endl;
            //throw std::exception("La falla!");
        }
        return L"";
    }


    //std::string Utf8Converter::ToUtf8(const std::wstring& widestring)
    std::string ToUtf8(const std::wstring& wideString)
    {
        size_t widesize = wideString.length();

        if (sizeof(wchar_t) == 2)
        {
            size_t utf8size = 3 * widesize + 1;
            char* stringnat = new char[utf8size];
            memset(stringnat, '\0', utf8size);
            //std::string resultString;
            //resultString.resize(utf8size, '\0');
            const UTF16* sourceStart = reinterpret_cast<const UTF16*>(wideString.c_str());
            const UTF16* sourceEnd = sourceStart + widesize;
            //UTF8* targetStart = reinterpret_cast<UTF8*>(&resultString[0]);
            UTF8* targetStart = reinterpret_cast<UTF8*>(stringnat);
            UTF8* targetEnd = targetStart + utf8size;
            ConversionResult res = ConvertUTF16toUTF8(&sourceStart, sourceEnd, &targetStart, targetEnd, strictConversion);
            if (res != conversionOK)
            {
                delete [] stringnat;
                std::cerr << "Blad konwersji!" << std::endl;
                //throw std::exception("La falla!");
            }
            *targetStart = 0;
            std::string resultString(stringnat);
            delete [] stringnat;
            return resultString;
        }
        else if (sizeof(wchar_t) == 4)
        {
//            std::cerr << "uinty: " << sizeof(uint8_t) << " " << sizeof(uint16_t) << " " << sizeof(uint32_t) << std::endl;
            size_t utf8size = 4 * widesize + 1;
            char* stringnat = new char[utf8size];
            memset(stringnat, '\0', utf8size);
            //std::string resultString;
            //resultString.resize(utf8size, '\0');
            const UTF32* sourceStart = reinterpret_cast<const UTF32*>(wideString.c_str());
            const UTF32* sourceEnd = sourceStart + widesize;
            //UTF8* targetStart = reinterpret_cast<UTF8*>(&resultString[0]);
            UTF8* targetStart = reinterpret_cast<UTF8*>(stringnat);
            UTF8* targetEnd = targetStart + utf8size;
            ConversionResult res = ConvertUTF32toUTF8(&sourceStart, sourceEnd, &targetStart, targetEnd, strictConversion);
            if (res != conversionOK)
            {
                delete [] stringnat;
                std::cerr << "Blad konwersji!" << std::endl;
                //throw std::exception("La falla!");
            }
            //*targetStart = (UTF8)0;
//            *targetStart = (char)0;
            std::string resultString(stringnat);
            delete [] stringnat;
            //int nullPos = resultString.find('\0');
            //if (nullPos != std::string::npos)
            //    resultString = resultString.substr(0, nullPos);
            return resultString;
        }
        else
        {
            std::cerr << "Blad konwersji!" << std::endl;
            //throw std::exception("La falla!");
        }
        return "";
    }

}

//int main(int argc, char **argv)
//{
//    std::ifstream szatan("simple.sr");
//    while (!szatan.eof())
//    {
//        std::string s;
//        getline(szatan, s);
//        std::wstring ws = FromUtf8(s);
//        std::cout << "Linia: " << s << std::endl;
//        std::cout << "Szeroka linia: " << ToUtf8(ws) << std::endl;
//        if (ws.find(L"a") != std::wstring::npos)
//            std::cout << "jest ay" << std::endl;
//        else
//            std::cout << "nie ma ay" << std::endl;
//    }
//    return 666;
//}

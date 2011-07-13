#ifndef ICUCONVERTER_H__
#define ICUCONVERTER_H__

#include <string>
#include <unicode/ustring.h>
#include <unicode/unistr.h>
#include <unicode/regex.h>

namespace icuConverter
{
    UnicodeString fromUtf(std::string utfString);
    std::string toUtf(UnicodeString uniString);
}


#endif


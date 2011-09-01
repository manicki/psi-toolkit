#ifndef REGEXP_HDR
#define REGEXP_HDR

#include "config.h"

#if HAVE_RE2
#include <re2/re2.h>
#endif
#if HAVE_PCRE
#include <pcrecpp.h>
#endif

#if HAVE_RE2
    typedef RE2 RegExp;
    typedef re2::StringPiece StringPiece;
#if HAVE_PCRE
    typedef pcrecpp::RE PerlRegExp;
#else
    typedef RE2 PerlRegExp;
#endif
#else
#if HAVE_PCRE
    typedef pcrecpp::RE RegExp;
    typedef pcrecpp::RE PerlRegExp;
#endif
#endif


#endif


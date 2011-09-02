#ifndef REGEXP_HDR
#define REGEXP_HDR

#include "config.h"

#if HAVE_RE2
#include <re2/re2.h>
#endif
#if HAVE_PCRE
#include <pcrecpp.h>
#include "regexp_variadic_function.hpp"
#endif

#if HAVE_PCRE
class PCREWrapper;
#endif

#if HAVE_RE2
    typedef RE2 RegExp;
    typedef re2::StringPiece StringPiece;
    typedef RE2::Arg Arg;
#if HAVE_PCRE
    typedef PCREWrapper PerlRegExp;
    typedef pcrecpp::StringPiece PerlStringPiece;
    typedef pcrecpp::Arg Arg;
#else
    typedef RE2 PerlRegExp;
    typedef re2::StringPiece PerlStringPiece;
    typedef RE2::Arg PerlArg;
#endif
#else
#if HAVE_PCRE
    typedef PCREWrapper RegExp;
    typedef PCREWrapper PerlRegExp;
    typedef pcrecpp::StringPiece StringPiece;
    typedef pcrecpp::Arg Arg;
    typedef pcrecpp::StringPiece PerlStringPiece;
    typedef pcrecpp::Arg PerlArg;
#endif
#endif

#if HAVE_PCRE
class PCREWrapper : private pcrecpp::RE {
    public:
        PCREWrapper(const std::string &pattern) :
            pcrecpp::RE(pattern, pcrecpp::UTF8()) {}
        PCREWrapper(const char *pattern) :
            pcrecpp::RE(pattern, pcrecpp::UTF8()) {}

        static bool FullMatchN(const StringPiece& text, const PCREWrapper& re,
                const Arg* const args[], int argc);
        static const VariadicFunction2<
            bool, const StringPiece&, const PCREWrapper&, Arg,
            PCREWrapper::FullMatchN> FullMatch;

        static bool PartialMatchN(const StringPiece& text, const PCREWrapper& re,
                const Arg* const args[], int argc);
        static const VariadicFunction2<
            bool, const StringPiece&, const PCREWrapper&, Arg,
            PCREWrapper::PartialMatchN> PartialMatch;

        static bool ConsumeN(StringPiece* input, const PCREWrapper& re,
                const Arg* const args[], int argc);
        static const VariadicFunction2<
            bool, StringPiece*, const PCREWrapper&, Arg,
            PCREWrapper::ConsumeN> Consume;

        static bool FindAndConsumeN(StringPiece* input, const PCREWrapper& re,
                const Arg* const args[], int argc);
        static const VariadicFunction2<
            bool, StringPiece*, const PCREWrapper&, Arg,
            PCREWrapper::FindAndConsumeN> FindAndConsume;

        static bool Replace(std::string *str,
                const PCREWrapper& pattern,
                const StringPiece& rewrite) {
            return ((pcrecpp::RE)pattern).Replace(rewrite, str);
        }

        static int GlobalReplace(std::string *str,
                const PCREWrapper& pattern,
                const StringPiece& rewrite) {
            return ((pcrecpp::RE)pattern).GlobalReplace(rewrite, str);
        }

        static bool Extract(const StringPiece &text,
                const PCREWrapper& pattern,
                const StringPiece &rewrite,
                std::string *out) {
            return ((pcrecpp::RE)pattern).Extract(rewrite, text, out);
        }

        int NumberOfCapturingGroups() const {
            return ((pcrecpp::RE)(*this)).NumberOfCapturingGroups();
        }

        bool ok() const {
            return true;
        }

        const string& pattern() const {
            return ((const pcrecpp::RE&)(*this)).pattern();
        }

        //@todo: any other methods of pcrecpp::RE ?
};
#endif

#endif


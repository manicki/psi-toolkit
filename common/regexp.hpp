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
    typedef pcrecpp::Arg PerlArg;
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

        static bool FullMatchN(const PerlStringPiece& text, const PCREWrapper& re,
                const PerlArg* const args[], int argc);
        static const VariadicFunction2<
            bool, const PerlStringPiece&, const PCREWrapper&, PerlArg,
            PCREWrapper::FullMatchN> FullMatch;

        static bool PartialMatchN(const PerlStringPiece& text, const PCREWrapper& re,
                const PerlArg* const args[], int argc);
        static const VariadicFunction2<
            bool, const PerlStringPiece&, const PCREWrapper&, PerlArg,
            PCREWrapper::PartialMatchN> PartialMatch;

        static bool ConsumeN(PerlStringPiece* input, const PCREWrapper& re,
                const PerlArg* const args[], int argc);
        static const VariadicFunction2<
            bool, PerlStringPiece*, const PCREWrapper&, PerlArg,
            PCREWrapper::ConsumeN> Consume;

        static bool FindAndConsumeN(PerlStringPiece* input, const PCREWrapper& re,
                const PerlArg* const args[], int argc);
        static const VariadicFunction2<
            bool, PerlStringPiece*, const PCREWrapper&, PerlArg,
            PCREWrapper::FindAndConsumeN> FindAndConsume;

        static bool Replace(std::string *str,
                const PCREWrapper& pattern,
                const PerlStringPiece& rewrite) {
            return ((pcrecpp::RE)pattern).Replace(rewrite, str);
        }

        static int GlobalReplace(std::string *str,
                const PCREWrapper& pattern,
                const PerlStringPiece& rewrite) {
            return ((pcrecpp::RE)pattern).GlobalReplace(rewrite, str);
        }

        static bool Extract(const PerlStringPiece &text,
                const PCREWrapper& pattern,
                const PerlStringPiece &rewrite,
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


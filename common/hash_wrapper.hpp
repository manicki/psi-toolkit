#ifndef HASH_WRAPPER_HDR
#define HASH_WRAPPER_HDR

#define USE_UNORDERED

#ifdef USE_UNORDERED

#include <tr1/unordered_map>
#include <tr1/unordered_set>

template<class A, class B>
struct HashWrapper {
    typedef std::tr1::unordered_map<A, B> type;
};

template<class A, class B, class C>
struct HashWrapper3 {
    typedef std::tr1::unordered_map<A, B, C> type;
};

template<class A>
struct HashWrapperSet {
    typedef std::tr1::unordered_set<A> type;
};

template<class A, class B>
struct HashWrapperSet2 {
    typedef std::tr1::unordered_set<A, B> type;
};

// #define HASH_WRAPPER_HASH_INTRO namespace std::tr1 {
#define HASH_WRAPPER_HASH_INTRO namespace std { namespace tr1 {

// #define HASH_WRAPPER_HASH_OUTRO };
#define HASH_WRAPPER_HASH_OUTRO } };

#define HASH_WRAPPER_HASH_TRAITS hash

#define HASH_WRAPPER_FULL_HASH_TRAITS std::tr1::hash

#define HASH_WRAPPER_EXTRA_STUFF

#elif __GNUC__


#include <ext/hash_map>
#include <ext/hash_set>

namespace __gnu_cxx {
    template<>
         struct hash<std::string> {
            size_t operator()(const std::string& s) const
            { return hash<char const*>()(s.c_str()); }
    };
};

template<class A, class B>
struct HashWrapper {
    typedef __gnu_cxx::hash_map<A, B> type;
};

template<class A, class B, class C>
struct HashWrapper3 {
    typedef __gnu_cxx::hash_map<A, B, C> type;
};

template<class A>
struct HashWrapperSet {
    typedef __gnu_cxx::hash_set<A> type;
};

template<class A, class B>
struct HashWrapperSet2 {
    typedef __gnu_cxx::hash_set<A, B> type;
};

#define HASH_WRAPPER_HASH_INTRO namespace __gnu_cxx {

#define HASH_WRAPPER_HASH_OUTRO };

#define HASH_WRAPPER_HASH_TRAITS hash

#define HASH_WRAPPER_FULL_HASH_TRAITS __gnu_cxx::hash

#define HASH_WRAPPER_EXTRA_STUFF


#else


#include <hash_map>
#include <hash_set>

template<class A, class B>
struct HashWrapper {
    typedef stdext::hash_map<A, B> type;
};

template<class A, class B, class C>
struct HashWrapper3 {
    typedef stdext::hash_map<A, B, C> type;
};

template<class A>
struct HashWrapperSet {
    typedef stdext::hash_set<A> type;
};

template<class A, class B>
struct HashWrapperSet2 {
    typedef stdext::hash_set<A, B> type;
};

#define HASH_WRAPPER_HASH_INTRO namespace stdext {

#define HASH_WRAPPER_HASH_OUTRO };

#define HASH_WRAPPER_HASH_TRAITS hash_compare

#define HASH_WRAPPER_FULL_HASH_TRAITS stdext::hash_compare

#define HASH_WRAPPER_EXTRA_STUFF \
static const size_t bucket_size = 4; \
static const size_t min_buckets = 8;


#endif


#endif

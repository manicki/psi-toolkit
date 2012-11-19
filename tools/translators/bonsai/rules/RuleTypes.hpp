#ifndef RULETYPES_HPP__
#define RULETYPES_HPP__

#include <vector>
#include <boost/tuple/tuple.hpp>

namespace poleng
{

namespace bonsai
{
    
namespace rules {

    typedef int State;    
    typedef unsigned int Position;
    typedef unsigned int Symbol;
    typedef unsigned int Weight;
    
    struct Arc { Symbol a; State q; Weight w; };
    typedef Arc* ArcIt;
    typedef std::pair<ArcIt, ArcIt> Range;
    
    typedef std::vector<int> Word;
    typedef boost::tuple<Word, int> WordIndex;
    typedef boost::tuple<Word, int, int> WordIndexHashed;
    
    typedef std::vector<WordIndex> WordList;
    typedef std::vector<WordIndexHashed> WordListHashed;
    
    typedef std::vector<WordList> StateLanguage;
    
    typedef boost::tuple<Word,Word,Word> WordTriple;
    typedef std::vector<WordTriple> WordTriples;
    
    struct WordIndexSorter {
        bool operator()(const WordIndex &w1, const WordIndex &w2) const {
            return w1.get<1>() < w2.get<1>();
        }
    };
    
}

}

}

#endif

#ifndef RECTGENERATOR_H__
#define RECTGENERATOR_H__

#include <set>
#include <map>

#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "RuleGraph.hpp"

namespace poleng
{

namespace bonsai
{

class RectCoord
{
    public:
        RectCoord(int ff_, int fs_, int sf_, int ss_, bool type_);
        RectCoord(Coord c0, Coord c1, bool type_);

        std::pair<int, int> first;
        std::pair<int, int> second;
        bool type;

    private:
};

//typedef std::pair<Coord, Coord> RectCoord;
typedef std::pair<int, int> TreeRange;
typedef std::map<std::string, std::set<std::string> > PhraseFunctionWordMap;

class RectGenerator
{
    private:
        bool *alignment;
        ParseGraphPtr graph;
        int s, t;
        std::vector<RectCoord> rects;
        std::vector<std::string> srcTokens;
        std::vector<std::string> trgTokens;
        PhraseFunctionWordMap pfmap;

    public:
        RectGenerator();
        RectGenerator(PhraseFunctionWordMap &);
        ~RectGenerator();

        std::vector<RectCoord> generate(std::string &srcSentence, std::string &trgSentence, std::string &groups, ParseGraphPtr pg);
};

}

}

#endif


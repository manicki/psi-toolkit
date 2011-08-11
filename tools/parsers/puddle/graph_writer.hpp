#ifndef GRAPHWRITER_H__
#define GRAPHWRITER_H__

#include <string>
#include <fstream>
#include "TransitionInfo.hpp"
#include "ParseGraph.hpp"

//#include "entity.hpp"

namespace poleng
{
namespace bonsai
{

namespace puddle
{

typedef boost::shared_ptr<std::string> StringPtr;

class GraphWriter
{
    public:
        GraphWriter();
        ~GraphWriter();
        void writeToFile(std::string filename);

        void setGraph(ParseGraphPtr aGraph);
        ParseGraphPtr getGraph();

    private:
        ParseGraphPtr graph;
};

}

}

}

#endif

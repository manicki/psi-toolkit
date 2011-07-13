
#include "graph_writer.hpp"

namespace poleng
{

namespace bonsai
{

namespace puddle
{

GraphWriter::GraphWriter()
{
//    graph = new ParseGraph;
}


GraphWriter::~GraphWriter()
{
//    delete graph;
}

void GraphWriter::writeToFile(std::string filename)
{
    if (!graph)
        return;

    std::ofstream output(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
    //default_writer dw;
    //graph_property_writer gw;
    //edge_property_writer epw;
    StringPtr s = StringPtr(new std::string);
    *s = graph->write_graphviz();
    output << *s;
    s.reset();
    //write_graphviz(output, graph, dw, epw, gw);
    output.close();
}

void GraphWriter::setGraph(ParseGraphPtr aGraph)
{
    graph = aGraph;
//    delete graph;
//    graph = new ParseGraph(*aGraph);
}

ParseGraphPtr GraphWriter::getGraph()
{
    return graph;
}

}

}

}

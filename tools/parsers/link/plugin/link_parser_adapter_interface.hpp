#ifndef LINK_PARSER_ADAPTER_INTERFACE_HDR
#define LINK_PARSER_ADAPTER_INTERFACE_HDR

#include <istream>
#include <ostream>
#include <string>

#include "plugin_adapter.hpp"


class LinkParserAdapterInterface : public PluginAdapter {
public:
    virtual ~LinkParserAdapterInterface() { }
/*
    virtual void init() = 0;
    virtual void init(std::string arg0, std::string arg1, std::string arg2) = 0;

    virtual void finalize() = 0;

    virtual void setRankDir(std::string dir) = 0;

    virtual int addNode(std::string id) = 0;
    virtual void setNodeLabel(int node, std::string label) = 0;
    virtual void setNodeColor(int node, std::string color) = 0;

    virtual int addEdge(int source, int target) = 0;
    virtual void setEdgeLabel(int node, std::string label) = 0;
    virtual void setEdgeColor(int node, std::string color) = 0;
*/
};


#endif

#ifndef LINK_PARSER_ADAPTER_IMPL_HDR
#define LINK_PARSER_ADAPTER_IMPL_HDR

#include "link_parser_adapter_interface.hpp"


class LinkParserAdapterImpl : public LinkParserAdapterInterface {
public:
    LinkParserAdapterImpl();

    virtual ~LinkParserAdapterImpl();
/*
    virtual void init();
    virtual void init(std::string arg0, std::string arg1, std::string arg2);

    virtual void finalize();

    virtual void setRankDir(std::string dir);

    virtual int addNode(std::string id);
    virtual void setNodeLabel(int node, std::string label);
    virtual void setNodeColor(int node, std::string color);

    virtual int addEdge(int source, int target);
    virtual void setEdgeLabel(int node, std::string label);
    virtual void setEdgeColor(int node, std::string color);

private:
    GVC_t * gvc_;
    Agraph_t * g_;

    int nCount_;
    int eCount_;

    std::map<int, Agnode_t*> nodes_;
    std::map<int, Agedge_t*> edges_;

    void init_context_();
    void init_graph_();

    void finalize_context_();
    void finalize_graph_();
*/
};


#endif

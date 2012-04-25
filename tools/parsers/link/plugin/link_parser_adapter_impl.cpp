#include "link_parser_adapter_impl.hpp"


LinkParserAdapterImpl::LinkParserAdapterImpl() { }


LinkParserAdapterImpl::~LinkParserAdapterImpl() { }

/*
void LinkParserAdapterImpl::init() {
    init_context_();
    init_graph_();
}


void LinkParserAdapterImpl::init(std::string arg0, std::string arg1, std::string arg2) {
    init();
    const char * const args[] = {
        arg0.c_str(),
        arg1.c_str(),
        arg2.c_str()
    };
    gvParseArgs(gvc_, sizeof(args)/sizeof(char*), (char**)args);
}


void LinkParserAdapterImpl::finalize() {
    gvLayoutJobs(gvc_, g_);
    gvRenderJobs(gvc_, g_);
    gvFreeLayout(gvc_, g_);
    finalize_graph_();
    finalize_context_();
}


void LinkParserAdapterImpl::init_context_() {
    gvc_ = gvContext();
}


void LinkParserAdapterImpl::init_graph_() {
    g_ = agopen((char*)"g", AGDIGRAPH);
}


void LinkParserAdapterImpl::finalize_context_() {
    if (gvc_) {
        gvFreeContext(gvc_);
        gvc_ = NULL;
    }
}


void LinkParserAdapterImpl::finalize_graph_() {
    if (g_) {
        agclose(g_);
        g_ = NULL;
    }
}


void LinkParserAdapterImpl::setRankDir(std::string dir) {
    agsafeset(g_, (char*)"rankdir", (char*)(dir.c_str()), (char*)"");
}


int LinkParserAdapterImpl::addNode(std::string id) {
    Agnode_t * n = agnode(g_, (char*)(id.c_str()));
    ++nCount_;
    nodes_.insert(std::pair<int, Agnode_t*>(nCount_, n));
    return nCount_;
}


void LinkParserAdapterImpl::setNodeLabel(int node, std::string label) {
    agsafeset(nodes_[node], (char*)"label", (char*)(label.c_str()), (char*)"");
}


void LinkParserAdapterImpl::setNodeColor(int node, std::string color) {
    agsafeset(nodes_[node], (char*)"color", (char*)(color.c_str()), (char*)"");
}


int LinkParserAdapterImpl::addEdge(int source, int target) {
    Agedge_t * e = agedge(g_, nodes_[source], nodes_[target]);
    ++eCount_;
    edges_.insert(std::pair<int, Agedge_t*>(eCount_, e));
    return eCount_;
}


void LinkParserAdapterImpl::setEdgeLabel(int edge, std::string label) {
    agsafeset(edges_[edge], (char*)"label", (char*)(label.c_str()), (char*)"");
}


void LinkParserAdapterImpl::setEdgeColor(int edge, std::string color) {
    agsafeset(edges_[edge], (char*)"color", (char*)(color.c_str()), (char*)"");
}
*/

// ==============================================

extern "C" LinkParserAdapterImpl * create() {
    return new LinkParserAdapterImpl;
}

extern "C" void destroy(LinkParserAdapterImpl * Tl) {
    delete Tl;
}

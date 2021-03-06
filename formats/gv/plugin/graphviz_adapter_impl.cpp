#include "graphviz_adapter_impl.hpp"


GraphvizAdapterImpl::GraphvizAdapterImpl() :
    gvc_(NULL),
    g_(NULL),
    nCount_(0),
    eCount_(0)
{ }


GraphvizAdapterImpl::~GraphvizAdapterImpl() {
    finalize_graph_();
    finalize_context_();
}


void GraphvizAdapterImpl::init() {
    init_context_();
    init_graph_();
}


void GraphvizAdapterImpl::init(std::string arg0, std::string arg1, std::string arg2) {
    init();
    const char * const args[] = {
        arg0.c_str(),
        arg1.c_str(),
        arg2.c_str()
    };
    gvParseArgs(gvc_, sizeof(args)/sizeof(char*), (char**)args);
}


void GraphvizAdapterImpl::finalize() {
    gvLayoutJobs(gvc_, g_);
    gvRenderJobs(gvc_, g_);
    gvFreeLayout(gvc_, g_);
    finalize_graph_();
    finalize_context_();
}


void GraphvizAdapterImpl::init_context_() {
    gvc_ = gvContext();
}


void GraphvizAdapterImpl::init_graph_() {
    g_ = agopen((char*)"g", AGDIGRAPH);
}


void GraphvizAdapterImpl::finalize_context_() {
    if (gvc_) {
        gvFreeContext(gvc_);
        gvc_ = NULL;
    }
}


void GraphvizAdapterImpl::finalize_graph_() {
    if (g_) {
        agclose(g_);
        g_ = NULL;
    }
}


void GraphvizAdapterImpl::setRankDir(std::string dir) {
    agsafeset(g_, (char*)"rankdir", (char*)(dir.c_str()), (char*)"");
}


int GraphvizAdapterImpl::addNode(std::string id) {
    Agnode_t * n = agnode(g_, (char*)(id.c_str()));
    ++nCount_;
    nodes_.insert(std::pair<int, Agnode_t*>(nCount_, n));
    return nCount_;
}


void GraphvizAdapterImpl::setNodeLabel(int node, std::string label) {
    agsafeset(nodes_[node], (char*)"label", (char*)(label.c_str()), (char*)"");
}


void GraphvizAdapterImpl::setNodeColor(int node, std::string color) {
    agsafeset(nodes_[node], (char*)"color", (char*)(color.c_str()), (char*)"");
}


void GraphvizAdapterImpl::setNodeStyle(int node, std::string style) {
    agsafeset(nodes_[node], (char*)"style", (char*)(style.c_str()), (char*)"");
}


int GraphvizAdapterImpl::addEdge(int source, int target) {
    Agedge_t * e = agedge(g_, nodes_[source], nodes_[target]);
    ++eCount_;
    edges_.insert(std::pair<int, Agedge_t*>(eCount_, e));
    return eCount_;
}


void GraphvizAdapterImpl::setEdgeLabel(int edge, std::string label) {
    agsafeset(edges_[edge], (char*)"label", (char*)(label.c_str()), (char*)"");
}


void GraphvizAdapterImpl::setEdgeColor(int edge, std::string color) {
    agsafeset(edges_[edge], (char*)"color", (char*)(color.c_str()), (char*)"");
}


void GraphvizAdapterImpl::setEdgeStyle(int edge, std::string style) {
    agsafeset(edges_[edge], (char*)"style", (char*)(style.c_str()), (char*)"");
}


// ==============================================

extern "C" GraphvizAdapterImpl * create() {
    return new GraphvizAdapterImpl;
}

extern "C" void destroy(GraphvizAdapterImpl * Tl) {
    delete Tl;
}

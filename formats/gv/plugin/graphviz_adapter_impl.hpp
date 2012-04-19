#ifndef GRAPHVIZ_ADAPTER_IMPL_HDR
#define GRAPHVIZ_ADAPTER_IMPL_HDR

#include "graphviz_adapter_interface.hpp"


class GraphvizAdapterImpl : public GraphvizAdapterInterface {
public:
    GraphvizAdapterImpl() { }

    virtual ~GraphvizAdapterImpl() { }

    virtual void convertDjVuToText(std::istream & djvuStream, std::iostream & textStream);
};


#endif

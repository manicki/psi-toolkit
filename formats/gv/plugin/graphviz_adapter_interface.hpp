#ifndef GRAPHVIZ_ADAPTER_INTERFACE_HDR
#define GRAPHVIZ_ADAPTER_INTERFACE_HDR

#include <istream>
#include <ostream>

#include "plugin_adapter.hpp"


class GraphvizAdapterInterface : public PluginAdapter {
public:
    virtual ~GraphvizAdapterInterface() { }

    virtual void convertDjVuToText(std::istream & djvuStream, std::iostream & textStream) = 0;
};


#endif

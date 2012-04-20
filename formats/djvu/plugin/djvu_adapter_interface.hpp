#ifndef DJVU_ADAPTER_INTERFACE_HDR
#define DJVU_ADAPTER_INTERFACE_HDR

#include <istream>
#include <ostream>

#include "plugin_adapter.hpp"


class DjVuAdapterInterface : public PluginAdapter {
public:
    virtual ~DjVuAdapterInterface() { }

    virtual void convertDjVuToText(std::istream & djvuStream, std::iostream & textStream) = 0;
};


#endif

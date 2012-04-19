#ifndef DJVU_ADAPTER_INTERFACE_HDR
#define DJVU_ADAPTER_INTERFACE_HDR

#include <string>

#include "plugin_adapter.hpp"


class DjVuAdapterInterface : public PluginAdapter {
public:
    virtual ~DjVuAdapterInterface() { }

    virtual void convertWordToText(std::string sourceFileName, std::string targetFileName) = 0;
};


#endif

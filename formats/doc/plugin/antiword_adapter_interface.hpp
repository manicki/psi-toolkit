#ifndef ANTIWORD_ADAPTER_INTERFACE_HDR
#define ANTIWORD_ADAPTER_INTERFACE_HDR

#include <string>

#include "plugin_adapter.hpp"


class AntiwordAdapterInterface : public PluginAdapter {
public:
    virtual ~AntiwordAdapterInterface() { }

    virtual void convertWordToText(std::string sourceFileName, std::string targetFileName) = 0;
};


#endif

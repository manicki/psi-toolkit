#ifndef LINK_PARSER_ADAPTER_INTERFACE_HDR
#define LINK_PARSER_ADAPTER_INTERFACE_HDR

#include "plugin_adapter.hpp"

#include <string>


class LinkParserAdapterInterface : public PluginAdapter {
public:
    virtual ~LinkParserAdapterInterface() { }

    virtual std::string parseSentence(std::string sentenceStr) = 0;
};


#endif

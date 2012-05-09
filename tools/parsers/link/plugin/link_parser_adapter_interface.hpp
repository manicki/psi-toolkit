#ifndef LINK_PARSER_ADAPTER_INTERFACE_HDR
#define LINK_PARSER_ADAPTER_INTERFACE_HDR

#include "plugin_adapter.hpp"

#include <string>
#include <vector>

#include "edge_description.hpp"


class LinkParserAdapterInterface : public PluginAdapter {
public:
    virtual ~LinkParserAdapterInterface() { }

    virtual void setDictionary(std::string filename) = 0;

    virtual std::vector<EdgeDescription> parseSentence(std::string sentenceStr) = 0;
};


#endif

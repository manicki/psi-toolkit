#ifndef LINK_PARSER_ADAPTER_INTERFACE_HDR
#define LINK_PARSER_ADAPTER_INTERFACE_HDR

#include "plugin_adapter.hpp"

#include <map>
#include <string>

#include "edge_description.hpp"


class LinkParserAdapterInterface : public PluginAdapter {
public:
    virtual ~LinkParserAdapterInterface() { }

    virtual void setDictionary(std::string language) = 0;

    virtual void setDictionary(
        std::string dictionaryName,
        std::string postProcessFileName,
        std::string constituentKnowledgeName,
        std::string affixName
    ) = 0;

    virtual std::map<int, EdgeDescription> parseSentence(std::string sentenceStr) = 0;
};


#endif

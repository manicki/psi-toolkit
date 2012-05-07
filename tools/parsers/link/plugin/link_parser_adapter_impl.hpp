#ifndef LINK_PARSER_ADAPTER_IMPL_HDR
#define LINK_PARSER_ADAPTER_IMPL_HDR

#include "link_parser_adapter_interface.hpp"

#include <map>

#include <link-includes.h>


class LinkParserAdapterImpl : public LinkParserAdapterInterface {
public:
    LinkParserAdapterImpl();

    virtual ~LinkParserAdapterImpl();

    virtual void setDictionary(std::string filename);

    virtual std::vector<EdgeDescription> parseSentence(std::string sentenceStr);

private:
    Dictionary dictionary_;
    Sentence sentence_;

    std::map<int, int> starts_;
    std::map<int, int> ends_;

    std::vector<EdgeDescription> extractEdgeDescriptions(CNode * ctree);

};


#endif

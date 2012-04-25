#ifndef LINK_PARSER_ADAPTER_IMPL_HDR
#define LINK_PARSER_ADAPTER_IMPL_HDR

#include "link_parser_adapter_interface.hpp"

#include <link-includes.h>


class LinkParserAdapterImpl : public LinkParserAdapterInterface {
public:
    LinkParserAdapterImpl();

    virtual ~LinkParserAdapterImpl();

    virtual void setDictionary(std::string filename);

    virtual std::string parseSentence(std::string sentenceStr);

private:
    Dictionary dictionary_;

};


#endif

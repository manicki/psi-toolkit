#ifndef LINK_PARSER_ADAPTER_IMPL_HDR
#define LINK_PARSER_ADAPTER_IMPL_HDR

#include "link_parser_adapter_interface.hpp"

#include <map>

#include <link-includes.h>


class LinkParserAdapterImpl : public LinkParserAdapterInterface {
public:
    LinkParserAdapterImpl();

    virtual ~LinkParserAdapterImpl();

    virtual void setDictionary(std::string language);

    virtual void setDictionary(
        std::string dictionaryName,
        std::string postProcessFileName,
        std::string constituentKnowledgeName,
        std::string affixName
    );

    virtual std::map<int, EdgeDescription> parseSentence(std::string sentenceStr);

private:
    Dictionary dictionary_;
    Sentence sentence_;

    std::map<int, int> starts_;
    std::map<int, int> ends_;

    std::map<int, EdgeDescription> edgeDescriptions_;

    int number_;

    int getNextNumber_();

    int extractEdgeDescriptions(CNode * ctree);

    void freeDictionary();
    void freeSentence();

};


#endif

#ifndef LINK_PARSER_PLUGIN_HDR
#define LINK_PARSER_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/link_parser_adapter_interface.hpp"


class LinkParserPlugin : public Plugin<LinkParserAdapterInterface> {
public:
    virtual std::string getLibraryName();
    virtual std::string getName();

private:
    virtual bool doCheckRequirementsWithOptionsDefaultOS(
        const boost::program_options::variables_map& options
    );

    virtual bool doCheckRequirementsWithOptionsUbuntu(
        const boost::program_options::variables_map& options
    );

    virtual bool areRequirementsDefinedForUbuntu();

    virtual bool doCheckRequirementsWithOptionsArchLinux(
        const boost::program_options::variables_map& options
    );

    virtual bool areRequirementsDefinedForArchLinux();
};


#endif

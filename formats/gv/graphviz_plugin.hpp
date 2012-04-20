#ifndef GRAPHVIZ_PLUGIN_HDR
#define GRAPHVIZ_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/graphviz_adapter_interface.hpp"


class GraphvizPlugin : public Plugin<GraphvizAdapterInterface> {
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
};


#endif

#ifndef POPPLER_PLUGIN_HDR
#define POPPLER_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/poppler_adapter_interface.hpp"


class PopplerPlugin : public Plugin<PopplerAdapterInterface> {
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

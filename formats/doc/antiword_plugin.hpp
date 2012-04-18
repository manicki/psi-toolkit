#ifndef ANTIWORD_PLUGIN_HDR
#define ANTIWORD_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/antiword_adapter_interface.hpp"


class AntiwordPlugin : public Plugin<AntiwordAdapterInterface> {
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

#ifndef DJVU_PLUGIN_HDR
#define DJVU_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/djvu_adapter_interface.hpp"


class DjVuPlugin : public Plugin<DjVuAdapterInterface> {
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

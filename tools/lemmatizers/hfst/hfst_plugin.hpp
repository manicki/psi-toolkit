#ifndef HFST_PLUGIN_HDR
#define HFST_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/hfst_adapter_interface.hpp"

class HfstPlugin : public Plugin<HfstAdapterInterface> {
public:
    virtual std::string getLibraryName();
    virtual std::string getName();

private:
    virtual bool doCheckRequirementsWithOptionsDefaultOS(
            const boost::program_options::variables_map& options
            );

    virtual bool doCheckRequirementsWithOptionsUbuntu(
            const boost::program_options::variables_map& options);
    virtual bool areRequirementsDefinedForUbuntu();

    virtual bool doCheckRequirementsWithOptionsArchLinux(
            const boost::program_options::variables_map& options);
    virtual bool areRequirementsDefinedForArchLinux();

    std::string getHfstRequiredLibraryName_();
};

#endif

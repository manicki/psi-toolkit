#ifndef SFST_PLUGIN_HDR
#define SFST_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/sfst_adapter_interface.hpp"

class SfstPlugin : public Plugin<SfstAdapterInterface> {
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

    std::string getSfstRequiredLibraryName_();
};

#endif

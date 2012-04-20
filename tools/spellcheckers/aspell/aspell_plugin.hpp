#ifndef ASPELL_PLUGIN_HDR
#define ASPELL_PLUGIN_HDR

#include "plugin.hpp"
#include "plugin/aspell_adapter_interface.hpp"

class AspellPlugin : public Plugin<AspellAdapterInterface> {
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

    bool checkIfDictionaryAvailable_(const boost::program_options::variables_map& options);
    std::string getAspellRequiredLibraryName_();
};

#endif

#include "hfst_plugin.hpp"
#include <boost/assign.hpp>

std::string HfstPlugin::getLibraryName() {
    return "libpsitoolkit_tools-lemmatizers-hfst-plugin.so";
}

std::string HfstPlugin::getName() {
    return "hfst";
}

bool HfstPlugin::doCheckRequirementsWithOptionsDefaultOS(
        const boost::program_options::variables_map& options) {

    if (! isPluginActive()) {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
                                        boost::assign::list_of
                                        (getHfstRequiredLibraryName_()));
        return false;
    }

    return true;
}

bool HfstPlugin::doCheckRequirementsWithOptionsUbuntu(
        const boost::program_options::variables_map& options) {

    if (! isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
                                        boost::assign::list_of
                                        ("hfst")
                                                                 );
        return false;
    }

    return true;
}

bool HfstPlugin::doCheckRequirementsWithOptionsArchLinux(
                   const boost::program_options::variables_map& options) {

    if (! isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
                                        boost::assign::list_of
                                        ("hfst"));
        return false;
    }

    return true;
}

bool HfstPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}

bool HfstPlugin::areRequirementsDefinedForArchLinux() {
    return true;
}

std::string HfstPlugin::getHfstRequiredLibraryName_() {
    return "libhfst.so";
}

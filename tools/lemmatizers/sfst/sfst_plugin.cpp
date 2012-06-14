#include "sfst_plugin.hpp"
#include <boost/assign.hpp>

std::string SfstPlugin::getLibraryName() {
    return "libpsitoolkit_tools-lemmatizers-sfst-plugin.so";
}

std::string SfstPlugin::getName() {
    return "sfst";
}

bool SfstPlugin::doCheckRequirementsWithOptionsDefaultOS(
    const boost::program_options::variables_map& /*options*/) {

    if (! isPluginActive()) {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
                                        boost::assign::list_of
                                        (getSfstRequiredLibraryName_()));
        return false;
    }

    return true;
}

bool SfstPlugin::doCheckRequirementsWithOptionsUbuntu(
    const boost::program_options::variables_map& /*options*/) {

    if (! isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
                                        boost::assign::list_of
                                        ("sfst")
                                                                 );
        return false;
    }

    return true;
}

bool SfstPlugin::doCheckRequirementsWithOptionsArchLinux(
    const boost::program_options::variables_map& /*options*/) {

    if (! isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
                                        boost::assign::list_of
                                        ("sfst"));
        return false;
    }

    return true;
}

bool SfstPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}

bool SfstPlugin::areRequirementsDefinedForArchLinux() {
    return true;
}


std::string SfstPlugin::getSfstRequiredLibraryName_() {
    return "libsfst.so";
}

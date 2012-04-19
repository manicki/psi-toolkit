#include "djvu_plugin.hpp"

#include <boost/assign.hpp>


std::string DjVuPlugin::getLibraryName() {
    return "libpsitoolkit_formats-djvu-reader-plugin.so";
}


std::string DjVuPlugin::getName() {
    return "djvu";
}


bool DjVuPlugin::doCheckRequirementsWithOptionsDefaultOS(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of("libdjvulibre.so")
        );
        return false;
    }
    return true;
}


bool DjVuPlugin::doCheckRequirementsWithOptionsUbuntu(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
            boost::assign::list_of("libdjvulibre21")("libdjvulibre-dev")
        );
        return false;
    }
    return true;
}


bool DjVuPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}

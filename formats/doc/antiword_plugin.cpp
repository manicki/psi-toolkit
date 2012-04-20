#include "antiword_plugin.hpp"

#include <cstdlib>

#include <boost/assign.hpp>


std::string AntiwordPlugin::getLibraryName() {
    return "libpsitoolkit_formats-doc-reader-plugin.so";
}


std::string AntiwordPlugin::getName() {
    return "antiword";
}


bool AntiwordPlugin::doCheckRequirementsWithOptionsDefaultOS(
    const boost::program_options::variables_map& /* options */
) {
    if (system("exec 2>/dev/null; antiword") || !isPluginActive()) {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of("antiword")
        );
        return false;
    }
    return true;
}


bool AntiwordPlugin::doCheckRequirementsWithOptionsUbuntu(
    const boost::program_options::variables_map& /* options */
) {
    if (system("exec 2>/dev/null; antiword") || !isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
            boost::assign::list_of("antiword")
        );
        return false;
    }
    return true;
}


bool AntiwordPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}

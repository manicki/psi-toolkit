#include "poppler_plugin.hpp"

#include <boost/assign.hpp>


std::string PopplerPlugin::getLibraryName() {
    return "libpsitoolkit_formats-pdf-reader-plugin.so";
}


std::string PopplerPlugin::getName() {
    return "poppler";
}


bool PopplerPlugin::doCheckRequirementsWithOptionsDefaultOS(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of
                ("libfreetype.so")
                ("libglib-2.0.so")
                ("libgobject-2.0.so")
                ("libgdk_pixbuf-2.0.so")
                ("libgdk-x11-2.0.so")
                ("libgtk-x11-2.0.so")
                ("libcairo.so")
                ("libpango-1.0.so")
                ("libatk-1.0.so")
                ("libpoppler.so")
                ("libpoppler-glib.so")
        );
        return false;
    }
    return true;
}


bool PopplerPlugin::doCheckRequirementsWithOptionsUbuntu(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
            boost::assign::list_of("libpoppler13")("libpoppler-glib6")
        );
        return false;
    }
    return true;
}


bool PopplerPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}

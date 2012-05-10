#include "link_parser_plugin.hpp"

#include <boost/assign.hpp>


std::string LinkParserPlugin::getLibraryName() {
    return "libpsitoolkit_tools-parsers-link-plugin.so";
}


std::string LinkParserPlugin::getName() {
    return "link-parser";
}


bool LinkParserPlugin::doCheckRequirementsWithOptionsDefaultOS(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
            boost::assign::list_of
                ("liblink-grammar.so")
        );
        return false;
    }
    return true;
}


bool LinkParserPlugin::doCheckRequirementsWithOptionsUbuntu(
    const boost::program_options::variables_map& /* options */
) {
    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
            boost::assign::list_of
                ("liblink-grammar4")
        );
        return false;
    }
    return true;
}


bool LinkParserPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}


bool LinkParserPlugin::doCheckRequirementsWithOptionsArchLinux(
    const boost::program_options::variables_map& /*options*/) {

    if (!isPluginActive()) {
        getCurrentRequirementsChecker().showPackagesNotAvailable(
            boost::assign::list_of
                ("link-grammar"));
        return false;
    }

    return true;
}

bool LinkParserPlugin::areRequirementsDefinedForArchLinux() {
    return true;
}

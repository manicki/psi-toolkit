#include "archlinux_plugin_requirements_checker.hpp"

#include <boost/foreach.hpp>

ArchLinuxPluginRequirementsChecker::ArchLinuxPluginRequirementsChecker(std::ostream & message):
    PluginRequirementsChecker(message)
{
}

ArchLinuxPluginRequirementsChecker::~ArchLinuxPluginRequirementsChecker() {
}

std::string ArchLinuxPluginRequirementsChecker::getCheckerName() {
    return "ArchLinux plugin requirements checker";
}

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

void ArchLinuxPluginRequirementsChecker::showPackagesNotAvailable(
                 const std::list<std::string> & packagesNames) {
    messageOutputStream_ << "The following packages are not installed:" << std::endl;
    BOOST_FOREACH(std::string name, packagesNames){
        messageOutputStream_ << "\t" << name << std::endl;
    }

    messageOutputStream_ << "You can run the following command to install them:" << std::endl;

    messageOutputStream_ << "\t" << "pacman -S";
    BOOST_FOREACH(std::string name, packagesNames){
        messageOutputStream_ << " " << name;
    }

    messageOutputStream_ << std::endl;
}

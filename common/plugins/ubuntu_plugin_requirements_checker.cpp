#include "ubuntu_plugin_requirements_checker.hpp"

#include <list>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

UbuntuPluginRequirementsChecker::UbuntuPluginRequirementsChecker(std::ostream & message):
    PluginRequirementsChecker(message)
{
}

UbuntuPluginRequirementsChecker::~UbuntuPluginRequirementsChecker() {
}

std::string UbuntuPluginRequirementsChecker::getCheckerName() {
    return "Ubuntu plugin requirements checker";
}

void UbuntuPluginRequirementsChecker::showPackagesNotAvailable(
                 const std::list<std::string> & packagesNames) {
    messageOutputStream_ << "The following packages are not installed:" << std::endl;
    BOOST_FOREACH(std::string name, packagesNames){
        messageOutputStream_ << "\t" << name << std::endl;
    }

    messageOutputStream_ << "You can run the following command to install them:" << std::endl;

    messageOutputStream_ << "\t" << "apt-get install";
    BOOST_FOREACH(std::string name, packagesNames){
        messageOutputStream_ << " " << name;
    }

    messageOutputStream_ << std::endl;
}

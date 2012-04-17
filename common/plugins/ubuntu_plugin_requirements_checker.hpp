#ifndef UBUNTU_PLUGIN_REQUIREMENTS_CHECKER_HDR
#define UBUNTU_PLUGIN_REQUIREMENTS_CHECKER_HDR

#include "plugin_requirements_checker.hpp"

class UbuntuPluginRequirementsChecker :
    public PluginRequirementsChecker {
public:
    UbuntuPluginRequirementsChecker(std::ostream & message);
    virtual ~UbuntuPluginRequirementsChecker();
    virtual std::string getCheckerName();

    virtual void showPackagesNotAvailable(const std::list<std::string> & packagesNames);
};

#endif

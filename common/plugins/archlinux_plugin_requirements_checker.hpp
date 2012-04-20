#ifndef ARCHLINUX_PLUGIN_REQUIREMENTS_CHECKER_HDR
#define ARCHLINUX_PLUGIN_REQUIREMENTS_CHECKER_HDR

#include "plugin_requirements_checker.hpp"

class ArchLinuxPluginRequirementsChecker :
    public PluginRequirementsChecker {
public:
    ArchLinuxPluginRequirementsChecker(std::ostream & message);
    virtual ~ArchLinuxPluginRequirementsChecker();
    virtual std::string getCheckerName();
};

#endif

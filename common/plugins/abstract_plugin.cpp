#include "abstract_plugin.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "plugin_requirements_checker.hpp"

#if PACKAGE_OS == PACKAGE_OS_UBUNTU
#include "ubuntu_plugin_requirements_checker.hpp"
#elif PACKAGE_OS == PACKAGE_OS_ARCHLINUX
#include "archlinux_plugin_requirements_checker.hpp"
#endif

bool AbstractPlugin::checkRequirements(
                     const boost::program_options::variables_map& options,
                     std::ostream & message) {

    if (isPluginActive()) {
        return doCheckRequirementsWithOptions(options, message);
    } else {
        if (doCheckRequirementsWithOptions(options, message)) {
            throw AbstractPlugin::InactivePluginException(getName());
        } else {
            return false;
        }
    }
}

AbstractPlugin::InactivePluginException::InactivePluginException(
    const std::string& pluginName)
    : PsiException(std::string("plugin is not active, but requirements are met: `") + pluginName + "`") {
}

bool AbstractPlugin::doCheckRequirementsWithOptions(
                             const boost::program_options::variables_map& options,
                             std::ostream & message) {

    bool requirementsChecked =  false;
    bool result;

#if PACKAGE_OS == PACKAGE_OS_UBUNTU
    if (areRequirementsDefinedForUbuntu()) {
        requirementsChecked = true;
        currentRequirementsChecker =
            boost::shared_ptr<PluginRequirementsChecker>(
                              new UbuntuPluginRequirementsChecker(message));
        result = doCheckRequirementsWithOptionsUbuntu(options);
    }
#elif PACKAGE_OS == PACKAGE_OS_ARCHLINUX
    if (areRequirementsDefinedForArchLinux()) {
        requirementsChecked = true;
        currentRequirementsChecker =
            boost::shared_ptr<PluginRequirementsChecker>(
                              new ArchLinuxPluginRequirementsChecker(message));
        result = doCheckRequirementsWithOptionsArchLinux(options);
    }
#endif

    if (!requirementsChecked) {
        currentRequirementsChecker =
            boost::shared_ptr<PluginRequirementsChecker>(
                              new PluginRequirementsChecker(message));
        result = doCheckRequirementsWithOptionsDefaultOS(options);
    }

    finishRequirementsChecker_();

    return result;
}

PluginRequirementsChecker & AbstractPlugin::getCurrentRequirementsChecker() {
    return *currentRequirementsChecker;
}


bool AbstractPlugin::doCheckRequirementsWithOptionsUbuntu(
                     const boost::program_options::variables_map& /*options*/
                             ) {
    return true;
}

bool AbstractPlugin::areRequirementsDefinedForUbuntu() {
    return false;
}

bool AbstractPlugin::doCheckRequirementsWithOptionsArchLinux(
                     const boost::program_options::variables_map& /*options*/
                             ) {
    return true;
}

bool AbstractPlugin::areRequirementsDefinedForArchLinux() {
    return false;
}

void AbstractPlugin::finishRequirementsChecker_() {
    getCurrentRequirementsChecker().finish();
    currentRequirementsChecker = boost::shared_ptr<PluginRequirementsChecker>();
}

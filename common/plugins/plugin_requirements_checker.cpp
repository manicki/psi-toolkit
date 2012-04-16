#include "plugin_requirements_checker.hpp"

#include <list>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

PluginRequirementsChecker::PluginRequirementsChecker(std::ostream & message):
    messageOutputStream_(message)
{
}

PluginRequirementsChecker::~PluginRequirementsChecker() {
}

PluginRequirementsChecker::UnsupportedOperationException::UnsupportedOperationException(
    const std::string& checkerName,
    const std::string& operationName)
    : PsiException(std::string("unsupported operation in: " + checkerName + ": `"
                               + operationName + "`")) {
}

std::string PluginRequirementsChecker::getCheckerName() {
    return "default OS plugin requirements checker";
}

void PluginRequirementsChecker::showLibrariesNotAvailable(
                                        const std::list<std::string> & libraryNames) {
    messageOutputStream_ << "Following libraries are not available:" << std::endl;
    BOOST_FOREACH(std::string libraryName, libraryNames){
        messageOutputStream_ << "\t" << libraryName << std::endl;
    }
}

void PluginRequirementsChecker::showFilesNotAvailable(
                                        const std::list<std::string> & fileNames) {
    messageOutputStream_ << "Following files are not available:" << std::endl;
    BOOST_FOREACH(std::string fileName, fileNames){
        messageOutputStream_ << "\t" << fileName << std::endl;
    }
}

void PluginRequirementsChecker::finish() {
}

void PluginRequirementsChecker::showPackagesNotAvailable(
                                const std::list<std::string> & /*packagesNames*/) {
    throw UnsupportedOperationException(getCheckerName(),
                                        "showPackagesNotAvailable");
}

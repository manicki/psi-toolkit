#ifndef PLUGIN_REQUIREMENTS_CHECKER_HDR
#define PLUGIN_REQUIREMENTS_CHECKER_HDR

#include <string>
#include <list>
#include "psi_exception.hpp"

class PluginRequirementsChecker {
public:
    PluginRequirementsChecker(std::ostream & message);
    virtual ~PluginRequirementsChecker();

    virtual void showLibrariesNotAvailable(const std::list<std::string> & libraryNames);
    virtual void showFilesNotAvailable(const std::list<std::string> & fileNames);
    virtual void showPackagesNotAvailable(const std::list<std::string> & packagesNames);
    virtual std::string getCheckerName();
    virtual void finish();

    class UnsupportedOperationException : public PsiException {
    public:
        UnsupportedOperationException(
                                      const std::string& checkerName,
                                      const std::string& operationName);

        virtual ~UnsupportedOperationException() throw() {}
    };
    
protected:
    std::ostream & messageOutputStream_;
};

#endif

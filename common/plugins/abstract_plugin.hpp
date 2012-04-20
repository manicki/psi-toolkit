#ifndef ABSTRACT_PLUGIN_HDR
#define ABSTRACT_PLUGIN_HDR

#include <boost/program_options/variables_map.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include "plugin_adapter.hpp"
#include "psi_exception.hpp"
#include "plugin_requirements_checker.hpp"

class AbstractPlugin {
public:
    AbstractPlugin() {
        active_ = false;
    }

    virtual ~AbstractPlugin() {
    }

    bool checkRequirements(const boost::program_options::variables_map& options,
                           std::ostream & message);

    virtual std::string getName() = 0;
    virtual std::string getLibraryName() = 0;

    virtual PluginAdapter * createAdapter() = 0;
    virtual void destroyAdapter(PluginAdapter * adapter) = 0;
    virtual void loadPlugin() = 0;

    bool isPluginActive() {
        return active_;
    }

    class InactivePluginException : public PsiException {
    public:
        InactivePluginException(const std::string& pluginName);

        virtual ~InactivePluginException() throw() {}
    };


protected:
    void setPluginActive(bool newState) {
        active_ = newState;
    }

    PluginRequirementsChecker & getCurrentRequirementsChecker();

private:

    bool doCheckRequirementsWithOptions(
                             const boost::program_options::variables_map& options,
                             std::ostream & message);

    virtual bool doCheckRequirementsWithOptionsDefaultOS(
                             const boost::program_options::variables_map& options
                             ) = 0;

    /**
     * Redefine those methods in derived classes
     * to define requirements for specific system.
     */

    virtual bool doCheckRequirementsWithOptionsUbuntu(
                             const boost::program_options::variables_map& options
                             );
    virtual bool areRequirementsDefinedForUbuntu();

    virtual bool doCheckRequirementsWithOptionsArchLinux(
                             const boost::program_options::variables_map& options
                             );
    virtual bool areRequirementsDefinedForArchLinux();


    void finishRequirementsChecker_();

    boost::shared_ptr<PluginRequirementsChecker> currentRequirementsChecker;
    bool active_;
};

#endif

#ifndef ABSTRACT_PLUGIN_HDR
#define ABSTRACT_PLUGIN_HDR

#include <boost/program_options/variables_map.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include "plugin_adapter.hpp"

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

protected:
    void setPluginActive(bool newState) {
        active_ = newState;
    }

private:

    virtual bool doCheckRequirementsWithOptions(
                             const boost::program_options::variables_map& options,
                             std::ostream & message) = 0;

    bool active_;
};

#endif

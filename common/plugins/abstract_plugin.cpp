#include "abstract_plugin.hpp"
#include "logging.hpp"

bool AbstractPlugin::checkRequirements(
                     const boost::program_options::variables_map& options,
                     std::ostream & message) {

    if (isPluginActive()) {
        return doCheckRequirementsWithOptions(options, message);
    } else {
        if (doCheckRequirementsWithOptions(options, message)) {
            // @todo Something wrong!
            // Plugin not active but requirements ok?
            return false;
        } else {
            return false;
        }
    }
}

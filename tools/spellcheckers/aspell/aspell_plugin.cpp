#include "aspell_plugin.hpp"

std::string AspellPlugin::getLibraryName() {
    return "libpsitoolkit_tools-spellcheckers-aspell-plugin.so";
}

std::string AspellPlugin::getName() {
    return "aspell";
}

bool AspellPlugin::doCheckRequirementsWithOptions(
                   const boost::program_options::variables_map& options,
                   std::ostream & message) {

    if (! isPluginActive()) {
        // @todo check libraries!
        message << "Ubuntu packages required:" << std::endl;
        message << "\tapt-get install aspell libaspell-dev libaspell15";
        return false;
    } else {
        AspellAdapterInterface * adapter = createSpecificAdapter_();

        bool result = true;

        if (options.count("lang")) {
            std::string langCode = options["lang"].as<std::string>();
            if (!adapter->isDictionaryAvailable(langCode)) {
                message << "Dictionary: " << langCode << " is not installed!" << std::endl;
                message << "\tapt-get install aspell-" << langCode;
                result = false;
            }
        }

        destroySpecificAdapter_(adapter);
        return result;
    }
}

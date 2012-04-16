#include "aspell_plugin.hpp"
#include <boost/assign.hpp>

std::string AspellPlugin::getLibraryName() {
    return "libpsitoolkit_tools-spellcheckers-aspell-plugin.so";
}

std::string AspellPlugin::getName() {
    return "aspell";
}

bool AspellPlugin::doCheckRequirementsWithOptionsDefaultOS(
                   const boost::program_options::variables_map& options
                   ) {

    if (! isPluginActive()) {
        // @todo check libraries!
        //        message << "Ubuntu packages required:" << std::endl;
        //        message << "\tapt-get install aspell libaspell-dev libaspell15";
        getCurrentRequirementsChecker().showLibrariesNotAvailable(
                                        boost::assign::list_of
                                        (getAspellRequiredLibraryName_()));
        return false;
    } else {
        bool result = true;
        if (!checkIfDictionaryAvailable_(options)) {
            std::string langCode = options["lang"].as<std::string>();
            getCurrentRequirementsChecker().showFilesNotAvailable(
                   boost::assign::list_of
                   ("dictionary file: " + langCode + ".dat" )
                                                                  );
            result = false;
        }

        return result;
    }
}

bool AspellPlugin::doCheckRequirementsWithOptionsUbuntu(
                   const boost::program_options::variables_map& options
                   ) {

    if (! isPluginActive()) {
        // @todo check libraries!
        //        message << "Ubuntu packages required:" << std::endl;
        //        message << "\tapt-get install aspell libaspell-dev libaspell15";
        getCurrentRequirementsChecker().showPackagesNotAvailable(
                                        boost::assign::list_of
                                        ("aspell")
                                        ("libaspell15")
                                                                 );
        return false;
    } else {
        bool result = true;
        if (!checkIfDictionaryAvailable_(options)) {
            std::string langCode = options["lang"].as<std::string>();
            getCurrentRequirementsChecker().showPackagesNotAvailable(
                                        boost::assign::list_of
                                        ("aspell-" + langCode)
                                                                     );
            result = false;
        }

        return result;
    }
    
    return true;
}

bool AspellPlugin::areRequirementsDefinedForUbuntu() {
    return true;
}

bool AspellPlugin::checkIfDictionaryAvailable_(
                   const boost::program_options::variables_map& options) {
    bool result = true;

    if (options.count("lang")) {
        AspellAdapterInterface * adapter = createSpecificAdapter_();

        std::string langCode = options["lang"].as<std::string>();
        if (!adapter->isDictionaryAvailable(langCode)) {
            result = false;
        }

        destroySpecificAdapter_(adapter);
    }

    return result;
}

std::string AspellPlugin::getAspellRequiredLibraryName_() {
    return "libaspell.so";
}

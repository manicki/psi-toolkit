#include "plugin_manager.hpp"

#include "config.hpp"

#if HAVE_ASPELL
#include "aspell_plugin.hpp"
#endif

Plugin_Manager::Plugin_Manager() {
#if HAVE_ASPELL
    registerPlugin(new AspellPlugin());
#endif
}

Plugin_Manager::~Plugin_Manager() {
}

PluginManager& PluginManager::getInstance() {
    static PluginManager instance;
    return instance;
}

PluginAdapter * PluginManager::createPluginAdapter(const std::string & pluginName) {
    if (checkIsPluginActiveWithMessage_(pluginName)) {
        return getPlugin_(pluginName).createPluginAdapter();
    } else {
        return NULL;
    }
}

void PluginManager::destroyPluginAdapter(const std::string & pluginName,
                                         PluginAdapter * adapterPointer) {
    if (checkIsPluginActive_(pluginName)) {
        AbstractPlugin & plugin = getPlugin_(pluginName);
        plugin.destroyAdapter(adapterPointer);
    }
}

PluginManager::UnknownPluginException::UnknownPluginException(
    const std::string& pluginName)
    : PsiException(std::string("unknown plugin `") + pluginName + "`") {
}


void PluginManager::registerPlugin_(const AbstractPlugin* pluginPointer) {
    pluginPointer->loadPlugin();

    if (! pluginPointer->isPluginActive()) {
        // @todo Maybe a message? (or maybe not!)
    }

    nameToPluginMap_[pluginPointer->getName()]
        = boost::shared_ptr<AbstractPlugin>(pluginPointer);
}

bool PluginManager::checkIsPluginActiveWithMessage_(const std::string & pluginName) {
    return checkIsPluginActiveWithMessage_(pluginName, true);
}

bool PluginManager::checkIsPluginActive_(const std::string & pluginName, bool withMessage) {
    AbstractPlugin & plugin = getPlugin_(pluginName);
    if (plugin.isPluginActive()) {
        return true;
    } else {
        if (withMessage) {
            // @todo
        }

        return false;
    }
}

AbstractPlugin & PluginManager::getPlugin_(const std::string & pluginName) {
    if (nameToPluginMap_.count(pluginName))
        return *nameToPluginMap_[pluginName];

    throw UnknownPluginException(pluginName);
}

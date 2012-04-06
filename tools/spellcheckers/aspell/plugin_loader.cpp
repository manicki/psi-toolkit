#include <dlfcn.h>

#include "plugin_loader.hpp"
#include "logging.hpp"

typedef AspellAdapterInterface* create_t();
typedef void destroy_t(AspellAdapterInterface*);

PluginLoader * PluginLoader::instance_ = NULL;

PluginLoader * PluginLoader::getInstance() {
    if (!instance_) {
        instance_ = new PluginLoader();
    }

    return instance_;
}

PluginLoader::PluginLoader() {
    loadLibrary("libpsitoolkit_tools-spellcheckers-aspell-adapter.so");
}

AspellAdapterInterface * PluginLoader::createAdapter() {
    DEBUG("Create Adapter");
    create_t* creat = (create_t*) dlsym(libraryHandle, "create");

    if (!creat) {
        ERROR("Cant get symbol create: " << dlerror());
    }

    DEBUG("Got symbol create");
    AspellAdapterInterface * result = creat();
    DEBUG("Got new adapter...");
    return result;
}

void PluginLoader::destroyAdapter(AspellAdapterInterface * adapter) {
    DEBUG("Destroy Adapter");
    destroy_t* destroy = (destroy_t*) dlsym(libraryHandle, "destroy");
    destroy(adapter);
}

void PluginLoader::loadLibrary(const std::string & name) {
    libraryHandle = dlopen(name.c_str(), RTLD_NOW);
    if (!libraryHandle) {
        ERROR("Could not load library: " << name
              << " Reason: " << dlerror());
    }
    DEBUG("Library loaded...");
}


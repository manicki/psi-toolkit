#ifndef PLUGIN_INTERFACE_HDR
#define PLUGIN_INTERFACE_HDR

#include <string>
#include "abstract_plugin.hpp"
#include "logging.hpp"

template <typename PluginAdapterClass>
class Plugin : public AbstractPlugin{
public:
    Plugin() {
        DEBUG("Plugin constructor");
        setPluginActive(false);
    }

    void loadPlugin() {
        if (loadLibrary_()) {
            setPluginActive(true);
        }
    }

    virtual ~Plugin() {
    }

    virtual PluginAdapter * createAdapter() {
        return createSpecificAdapter_();
    }

    virtual destroyAdapter(PluginAdapter * adapter) {
        PluginAdapterClass * adapterSpecific =
            dynamic_cast<PluginAdapterClass>(adapter);
        destroySpecificAdapter_(adapterSpecific);
    }

protected:
    typedef PluginAdapterClass* createFunctionPointer();
    typedef void destroyFunctionPointer(PluginAdapterClass*);

    PluginAdapterClass * createSpecificAdapter_() {
        DEBUG("Create Adapter");
        createFunctionPointer* creat = (createFunctionPointer*) dlsym(libraryHandle, "create");

        if (!creat) {
            ERROR("Cant get symbol create: " << dlerror());
        }

        DEBUG("Got symbol create");
        PluginAdapterClass * result = creat();
        DEBUG("Got new adapter...");
        return result;
    }

    void destroySpecificAdapter_(PluginAdapterClass * adapter) {
        DEBUG("Destroy Adapter");
        destroyFunctionPointer* destroy =
            (destroyFunctionPointer*) dlsym(libraryHandle, "destroy");
        destroy(adapter);
    }
private:
    bool loadLibrary_() {
        libraryHandle_ = dlopen(getLibraryName().c_str(), RTLD_NOW);
        if (!libraryHandle_) {
            ERROR("Could not load library: " << libraryName_
                  << " Reason: " << dlerror());
            return false;
        }

        DEBUG("Library loaded...");
        return true;
    }

    void * libraryHandle_ = NULL;
};

#endif

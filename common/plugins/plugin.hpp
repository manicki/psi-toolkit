#ifndef PLUGIN_INTERFACE_HDR
#define PLUGIN_INTERFACE_HDR

#include <string>
#include "abstract_plugin.hpp"
#include "logging.hpp"
#include <dlfcn.h>

template <typename PluginAdapterClass>
class Plugin : public AbstractPlugin{
public:
    Plugin() : libraryHandle_(NULL) {
        setPluginActive(false);
    }

    virtual void loadPlugin() {
        setPluginActive(false);
        if (loadLibrary_()) {
            setPluginActive(true);
        }
    }

    virtual ~Plugin() {
    }

    virtual PluginAdapter * createAdapter() {
        return createSpecificAdapter_();
    }

    virtual void destroyAdapter(PluginAdapter * adapter) {
        PluginAdapterClass * adapterSpecific =
            dynamic_cast<PluginAdapterClass*>(adapter);
        destroySpecificAdapter_(adapterSpecific);
    }

protected:
    typedef PluginAdapterClass* createFunctionPointer();
    typedef void destroyFunctionPointer(PluginAdapterClass*);

    PluginAdapterClass * createSpecificAdapter_() {
        createFunctionPointer* creat = (createFunctionPointer*) dlsym(libraryHandle_, "create");

        if (!creat) {
            ERROR("Cant get symbol create: " << dlerror());
        }

        PluginAdapterClass * result = creat();
        return result;
    }

    void destroySpecificAdapter_(PluginAdapterClass * adapter) {
        destroyFunctionPointer* destroy =
            (destroyFunctionPointer*) dlsym(libraryHandle_, "destroy");
        destroy(adapter);
    }
private:
    bool loadLibrary_() {
        libraryHandle_ = dlopen(getLibraryName().c_str(), RTLD_NOW);
        if (!libraryHandle_) {
            DEBUG("Could not load library: " << getLibraryName()
                  << " Reason: " << dlerror());
            return false;
        }

        return true;
    }

    void * libraryHandle_;
};

#endif

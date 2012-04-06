#ifndef PLUGIN_LOADER_HDR
#define PLUGIN_LOADER_HDR

#include <string>
#include "adapter/aspell_adapter_interface.hpp"

class PluginLoader {
public:
    PluginLoader();
    AspellAdapterInterface * createAdapter();
    void destroyAdapter(AspellAdapterInterface * adapter);
    static PluginLoader * getInstance();
    
private:
    void loadLibrary(const std::string & name);

    void * libraryHandle;

    static PluginLoader * instance_;
};

#endif

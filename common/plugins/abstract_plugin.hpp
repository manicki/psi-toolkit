#ifndef ABSTRACT_PLUGIN_HDR
#define ABSTRACT_PLUGIN_HDR

#include <string>
#include "plugin_adapter.hpp"

template <typename AdapterInterfaceClass>
class AbstractPlugin {
public:
    virtual ~AbstractPlugin() {
    }

    virtual std::string getName() = 0;
    virtual std::string getLibraryName() = 0;

    virtual PluginAdapter * createAdapter() = 0;
    virtual destroyAdapter(PluginAdapter * adapter) = 0;

    bool isPluginActive() {
        return active_;
    }

protected:
    void setPluginActive(bool newState) {
        active_ = newState;
    }

private:
    bool active_ = false;
};

#endif

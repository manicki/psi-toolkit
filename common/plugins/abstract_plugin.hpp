#ifndef ABSTRACT_PLUGIN_HDR
#define ABSTRACT_PLUGIN_HDR

#include <string>
#include "plugin_adapter.hpp"

class AbstractPlugin {
public:
    AbstractPlugin() {
        active_ = false;
    }

    virtual ~AbstractPlugin() {
    }

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
    bool active_;
};

#endif

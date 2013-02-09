#ifndef HFST_ADAPTER_INTERFACE_HDR
#define HFST_ADAPTER_INTERFACE_HDR

#include <string>
#include <list>
#include <vector>

#include "plugin_adapter.hpp"

struct HfstPath {
    float weight;
    std::vector<std::string> path;
};

typedef std::list<HfstPath> HfstPaths;

class HfstAdapterInterface : public PluginAdapter {
public:
    virtual ~HfstAdapterInterface() {
    }

    virtual void initHfst(const std::string& filename) = 0;
    virtual HfstPaths analyzeWord(const std::string& word) = 0;
};

#endif

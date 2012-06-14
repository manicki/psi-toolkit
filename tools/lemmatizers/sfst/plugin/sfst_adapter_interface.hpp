#ifndef SFST_ADAPTER_INTERFACE_HDR
#define SFST_ADAPTER_INTERFACE_HDR

#include <string>

#include "plugin_adapter.hpp"

class SfstAdapterInterface : public PluginAdapter {
public:
    virtual ~SfstAdapterInterface() {
    }

    virtual void initSfst(FILE* fileName) = 0;
    virtual bool analyzeString(const std::string& word, FILE* outputFile) = 0;
};

#endif

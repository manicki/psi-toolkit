#ifndef POPPLER_ADAPTER_INTERFACE_HDR
#define POPPLER_ADAPTER_INTERFACE_HDR

#include <istream>
#include <ostream>

#include "plugin_adapter.hpp"


class PopplerAdapterInterface : public PluginAdapter {
public:
    virtual ~PopplerAdapterInterface() { }

    virtual void convertPDFToText(std::istream & pdfStream, std::iostream & textStream) = 0;
};


#endif

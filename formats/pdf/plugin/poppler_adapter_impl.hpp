#ifndef POPPLER_ADAPTER_IMPL_HDR
#define POPPLER_ADAPTER_IMPL_HDR

#include "poppler_adapter_interface.hpp"


class PopplerAdapterImpl : public PopplerAdapterInterface {
public:
    PopplerAdapterImpl() { }

    virtual ~PopplerAdapterImpl() { }

    virtual void convertPDFToText(std::istream & pdfStream, std::iostream & textStream);
};


#endif

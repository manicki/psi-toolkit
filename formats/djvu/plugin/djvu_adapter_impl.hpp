#ifndef DJVU_ADAPTER_IMPL_HDR
#define DJVU_ADAPTER_IMPL_HDR

#include "djvu_adapter_interface.hpp"


class DjVuAdapterImpl : public DjVuAdapterInterface {
public:
    DjVuAdapterImpl() { }

    virtual ~DjVuAdapterImpl() { }

    virtual std::iostream convertDjVuToText(std::istream djvuStream);
};


#endif

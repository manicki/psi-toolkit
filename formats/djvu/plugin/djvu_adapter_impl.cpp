#include "djvu_adapter_impl.hpp"


std::iostream DjVuAdapterImpl::convertDjVuToText(std::istream djvuStream) {
    //TODO
}


// ==============================================

extern "C" DjVuAdapterImpl * create() {
    return new DjVuAdapterImpl;
}

extern "C" void destroy(DjVuAdapterImpl * Tl) {
    delete Tl;
}

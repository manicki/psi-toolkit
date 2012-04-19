#include "djvu_adapter_impl.hpp"


void DjVuAdapterImpl::convertWordToText(
    std::string sourceFileName,
    std::string targetFileName
) {
    //TODO
}


// ==============================================

extern "C" DjVuAdapterImpl * create() {
    return new DjVuAdapterImpl;
}

extern "C" void destroy(DjVuAdapterImpl * Tl) {
    delete Tl;
}

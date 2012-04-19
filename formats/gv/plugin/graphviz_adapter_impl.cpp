#include "graphviz_adapter_impl.hpp"

// Fixes warnings "... not defined" from GraphViz include files

#ifndef _BLD_cdt
#define _BLD_cdt 0
#endif

#ifndef _DLL_BLD
#define _DLL_BLD 0
#endif

#ifndef _dll_import
#define _dll_import 0
#endif

#ifndef _PACKAGE_ast
#define _PACKAGE_ast 0
#endif

#include <gvc.h>


void GraphvizAdapterImpl::convertDjVuToText(std::istream & djvuStream, std::iostream & textStream) {
    //TODO
}


// ==============================================

extern "C" GraphvizAdapterImpl * create() {
    return new GraphvizAdapterImpl;
}

extern "C" void destroy(GraphvizAdapterImpl * Tl) {
    delete Tl;
}

#include "poppler_adapter_impl.hpp"

#include <limits>
#include <sstream>

#include <gtk/gtk.h>
#include <poppler.h>


void PopplerAdapterImpl::convertPDFToText(std::istream & pdfStream, std::iostream & textStream) {
    std::string line;
    std::string documentStr;
    while (std::getline(pdfStream, line)) {
        documentStr += line + "\n";
    }
    gtk_init_check(NULL, NULL);
    PopplerDocument * document = poppler_document_new_from_data(
        (char*)(documentStr.c_str()),
        documentStr.length(),
        NULL,
        NULL
    );
    PopplerPage * page;
    for (int i = 0; i < poppler_document_get_n_pages(document); ++i) {
        page = poppler_document_get_page(document, i);
#if POPPLER_CHECK_VERSION(0,16,0)
        char * text = poppler_page_get_text(page);
#else
        PopplerRectangle rect;
        double width;
        double height;
        poppler_page_get_size(page, &width, &height);
        rect.x1 = 0;
        rect.y1 = 0;
        rect.x2 = width;
        rect.y2 = height;
        char * text = poppler_page_get_text(page, POPPLER_SELECTION_GLYPH, &rect);
#endif
        textStream << text << std::endl;
    }
}


// ==============================================

extern "C" PopplerAdapterImpl * create() {
    return new PopplerAdapterImpl;
}

extern "C" void destroy(PopplerAdapterImpl * Tl) {
    delete Tl;
}

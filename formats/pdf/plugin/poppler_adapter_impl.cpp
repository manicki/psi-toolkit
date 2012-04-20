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
    gtk_init(NULL, NULL);
    PopplerDocument * document = poppler_document_new_from_data(
        (char*)(documentStr.c_str()),
        documentStr.length(),
        NULL,
        NULL
    );
    PopplerPage * page;
    for (int i = 0; i < poppler_document_get_n_pages(document); ++i) {
        page = poppler_document_get_page(document, i);
        char * text = poppler_page_get_text(page);
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

#include "djvu_adapter_impl.hpp"

#include <limits>
#include <sstream>

#include <libdjvu/ddjvuapi.h>
#include <libdjvu/miniexp.h>


void DjVuAdapterImpl::convertDjVuToText(std::istream & djvuStream, std::iostream & textStream) {

    ddjvu_context_t * context = ddjvu_context_create("djvu-reader");
    ddjvu_document_t * document = ddjvu_document_create(context, NULL, 0);

    ddjvu_message_t * message;

    message = ddjvu_message_wait(context);
    if (message->m_any.tag == DDJVU_NEWSTREAM) {
        int streamid = message->m_newstream.streamid;
        const unsigned short length = (std::numeric_limits<unsigned short>::max)();
        char * buffer = new char[length];
        while (djvuStream.good()) {
            djvuStream.read(buffer, length);
            ddjvu_stream_write(document, streamid, buffer, length);
        }
        ddjvu_stream_close(document, streamid, 0);
        delete [] buffer;
    }
    ddjvu_message_pop(context);

    miniexp_t me;

    message = ddjvu_message_wait(context);
    if (message->m_any.tag == DDJVU_DOCINFO) {
        int numPages = ddjvu_document_get_pagenum(document);
        for (int i = 0; i < numPages; ++i) {
            while ((me = ddjvu_document_get_pagetext(document, i, "page")) == miniexp_dummy) {
                ddjvu_message_wait(context);
                ddjvu_message_pop(context);
            }
            char * text;
            if (me && (text = (char*)(miniexp_to_str(miniexp_nth(5, me))))) {
                textStream << text << std::endl;
            }
            ddjvu_miniexp_release(document, me);
        }
    };
    ddjvu_message_pop(context);

    ddjvu_document_release(document);
    ddjvu_context_release(context);

}


// ==============================================

extern "C" DjVuAdapterImpl * create() {
    return new DjVuAdapterImpl;
}

extern "C" void destroy(DjVuAdapterImpl * Tl) {
    delete Tl;
}

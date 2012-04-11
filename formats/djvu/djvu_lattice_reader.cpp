#include "djvu_lattice_reader.hpp"

#include <sstream>

#include <boost/assign/list_of.hpp>

#include "libdjvu/miniexp.h"

#include "logging.hpp"


std::string DjVuLatticeReader::getFormatName() {
    return "DjVu";
}

std::string DjVuLatticeReader::doInfo() {
    return "DjVu reader";
}

DjVuLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* DjVuLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new DjVuLatticeReader();
}

boost::program_options::options_description DjVuLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string DjVuLatticeReader::Factory::doGetName() const {
    return "djvu-reader";
}

boost::filesystem::path DjVuLatticeReader::Factory::doGetFile() const {
    return __FILE__;
}

DjVuLatticeReader::Worker::Worker(
    DjVuLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("djvu-reader")))
{ }


void DjVuLatticeReader::Worker::handle_ddjvu_messages(ddjvu_context_t *ctx, int wait) {
    const ddjvu_message_t *msg;
    if (wait) {
        ddjvu_message_wait(ctx);
    }
    while ((msg = ddjvu_message_peek(ctx))) {
DEBUG("!" << msg->m_any.tag);
        switch(msg->m_any.tag) {
            case DDJVU_ERROR:      DEBUG("ERROR"); throw FileFormatException("DDJVU ERROR"); break;
            case DDJVU_INFO:       DEBUG("INFO"); break;
            case DDJVU_NEWSTREAM:  DEBUG("NEWSTREAM"); break;
            default: break;
        }
        ddjvu_message_pop(ctx);
    }
}


void DjVuLatticeReader::Worker::doRun() {

    // SET_LOGGING_LEVEL("DEBUG");

    ddjvu_context_t * context;
    if (!(context = ddjvu_context_create("djvu-reader"))) {
        throw FileFormatException("Cannot create DjVu context");
    }

    ddjvu_document_t * document;
    if (!(document = ddjvu_document_create(context, NULL, 0))) {
        throw FileFormatException("Cannot open DjVu document");
    }

    std::string line;
    while (std::getline(inputStream_, line)) {
        ddjvu_stream_write(document, 0, line.c_str(), line.length());
    }
/*
    while (!ddjvu_document_decoding_done(document)) {
        handle_ddjvu_messages(context, TRUE);
    }

    int npages = ddjvu_document_get_pagenum(document);
    DEBUG("---" << npages << "---");

    miniexp_t expression;
    for (int i = 0; i < npages; ++i) {
        while ((expression = ddjvu_document_get_pagetext(document, i, "word")) != miniexp_dummy) {
            handle_ddjvu_messages(context, TRUE);
            DEBUG("NAM[" << miniexp_to_name(expression) << "]");
            DEBUG("STR[" << miniexp_to_str(expression) << "]");
        }
        DEBUG(miniexp_to_str(expression));
    }
*/
    if (document) {
        ddjvu_document_release(document);
    }
    if (context) {
        ddjvu_context_release(context);
    }

}

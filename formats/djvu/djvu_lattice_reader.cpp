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


void DjVuLatticeReader::Worker::doRun() {

    ddjvu_context_t * context = ddjvu_context_create("djvu-reader");
    ddjvu_document_t * document = ddjvu_document_create(context, NULL, 0);

    ddjvu_message_t * message;

    message = ddjvu_message_wait(context);
    if (message->m_any.tag == DDJVU_NEWSTREAM) {
        int streamid = message->m_newstream.streamid;
        const unsigned short length = (std::numeric_limits<unsigned short>::max)();
        char * buffer = new char[length];
        while (inputStream_.good()) {
            inputStream_.read(buffer, length);
            ddjvu_stream_write(document, streamid, buffer, length);
        }
        ddjvu_stream_close(document, streamid, 0);
        delete [] buffer;
    }
    ddjvu_message_pop(context);

    miniexp_t me;
    char * text;

    message = ddjvu_message_wait(context);
    if (message->m_any.tag == DDJVU_DOCINFO) {
        int numPages = ddjvu_document_get_pagenum(document);
        for (int i = 0; i < numPages; ++i) {
            while ((me = ddjvu_document_get_pagetext(document, i, "page")) == miniexp_dummy) {
                ddjvu_message_wait(context);
                ddjvu_message_pop(context);
            }
            if (me && (text = (char*)(miniexp_to_str(miniexp_nth(5, me))))) {
                std::stringstream textStream(text);
                std::string line;
                while (getline(textStream, line)) {
                    appendParagraphToLattice_(line, textTags_);
                    lattice_.appendString("\n");
                }
            }
            ddjvu_miniexp_release(document, me);
        }
    };
    ddjvu_message_pop(context);

    ddjvu_document_release(document);
    ddjvu_context_release(context);

}

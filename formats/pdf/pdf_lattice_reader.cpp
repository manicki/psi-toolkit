#include "pdf_lattice_reader.hpp"

#include <sstream>

#include <boost/assign/list_of.hpp>

#include <gtk/gtk.h>
#include <poppler.h>

#include "logging.hpp"


std::string PDFLatticeReader::getFormatName() {
    return "PDF";
}

std::string PDFLatticeReader::doInfo() {
    return "PDF reader";
}

PDFLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* PDFLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new PDFLatticeReader();
}

boost::program_options::options_description PDFLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string PDFLatticeReader::Factory::doGetName() {
    return "pdf-reader";
}

boost::filesystem::path PDFLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

PDFLatticeReader::Worker::Worker(
    PDFLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("pdf-reader")))
{ }

void PDFLatticeReader::Worker::doRun() {
    std::string line;
    std::string documentStr;
    while (std::getline(inputStream_, line)) {
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
        std::stringstream textStream(text);
        while (getline(textStream, line)) {
            appendParagraphToLattice_(line, textTags_);
            lattice_.appendString("\n");
        }
    }
}

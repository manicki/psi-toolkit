#include "pdf_lattice_reader.hpp"

#include "logging.hpp"

#include <boost/assign/list_of.hpp>

std::string PDFLatticeReader::getFormatName() {
    return "PDF";
}

std::string PDFLatticeReader::doInfo() {
    return "PDF reader";
}

PDFLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* PDFLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map&) {
        // TODO implement program options
    return new PDFLatticeReader();
}

boost::program_options::options_description PDFLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("line-by-line", "processes line by line")
        ("whole-text",   "read the whole text")
        ("paragraphs",   "paragraphs are delimited with double newlines")
        ("discard-comments", "discards comments")
        ("pass-through-comments", "marks comments as single markup")
        ;

    return optionsDescription;
}

std::string PDFLatticeReader::Factory::doGetName() {
    return "pdf-reader";
}

boost::filesystem::path PDFLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

PDFLatticeReader::Worker::Worker(PDFLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
                  boost::assign::list_of("frag")("pdf-reader"))) {
}

void PDFLatticeReader::Worker::doRun() {
    std::string line;
    while (getline(inputStream_, line)) {
        appendParagraphToLattice_(line);
        lattice_.appendString("\n");
    }
}

void PDFLatticeReader::Worker::appendParagraphToLattice_(std::string paragraph) {
    Lattice::VertexDescriptor prevEnd = lattice_.getLastVertex();
    lattice_.appendStringWithSymbols(paragraph);
    Lattice::VertexDescriptor nowEnd = lattice_.getLastVertex();

    AnnotationItem item("FRAG", paragraph);
    lattice_.addEdge(prevEnd, nowEnd,
                     item, textTags_);

}

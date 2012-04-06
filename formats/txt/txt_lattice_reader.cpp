#include "txt_lattice_reader.hpp"

#include "logging.hpp"

#include <boost/assign/list_of.hpp>

std::string TxtLatticeReader::getFormatName() {
    return "TXT";
}

std::string TxtLatticeReader::doInfo() {
    return "plain text reader";
}

TxtLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* TxtLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map&) {
        // TODO implement program options
    return new TxtLatticeReader();
}

boost::program_options::options_description TxtLatticeReader::Factory::doOptionsHandled() {
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

std::string TxtLatticeReader::Factory::doGetName() const {
    return "txt-reader";
}

boost::filesystem::path TxtLatticeReader::Factory::doGetFile() const {
    return __FILE__;
}

TxtLatticeReader::Worker::Worker(TxtLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
                  boost::assign::list_of("frag")("txt-reader"))) {
}

void TxtLatticeReader::Worker::doRun() {
    DEBUG("READING TEXT");

    std::string line;
    while (getline(inputStream_, line)) {
        appendParagraphToLattice_(line);
        lattice_.appendString("\n");
    }
}

void TxtLatticeReader::Worker::appendParagraphToLattice_(std::string paragraph) {
    if (!paragraph.empty()) {
        Lattice::VertexDescriptor prevEnd = lattice_.getLastVertex();
        lattice_.appendStringWithSymbols(paragraph);
        Lattice::VertexDescriptor nowEnd = lattice_.getLastVertex();
        AnnotationItem item("FRAG", paragraph);
        lattice_.addEdge(prevEnd, nowEnd, item, textTags_);
    }
}

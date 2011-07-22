#include "txt_lattice_reader.hpp"


std::string TxtLatticeReader::getFormatName() {
    return "TXT";
}

std::string TxtLatticeReader::doInfo() {
    return "plain text reader";
}

LatticeReader* TxtLatticeReader::Factory::doCreateLatticeReader(
    boost::program_options::variables_map options) {
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

std::string TxtLatticeReader::Factory::doGetName() {
    return "txt-reader";
}


TxtLatticeReader::Worker::Worker(TxtLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker(inputStream, lattice), processor_(processor) {
}

void TxtLatticeReader::Worker::doRun() {
    std::cerr << "READING\n";

    std::string line;
    while (getline(inputStream_, line)) {
        appendParagraphToLattice_(line);
    }
}

void TxtLatticeReader::Worker::appendParagraphToLattice_(std::string paragraph) {
    lattice_.appendString(paragraph);
}

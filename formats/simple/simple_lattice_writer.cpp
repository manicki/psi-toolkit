#include "simple_lattice_writer.hpp"

#include <iomanip>
#include <iostream>

#include "logging.hpp"

std::string SimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter* SimpleLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map&) {
    return new SimpleLatticeWriter();
}

boost::program_options::options_description SimpleLatticeWriter::Factory::doOptionsHandled() {
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


std::string SimpleLatticeWriter::Factory::doGetName() {
    return "simple-writer";
}


std::string SimpleLatticeWriter::doInfo() {
    return "Simple writer";
}

SimpleLatticeWriter::Worker::Worker(SimpleLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    WriterWorker(outputStream, lattice), processor_(processor) {
}

void SimpleLatticeWriter::Worker::doRun() {

    //TODO

}

SimpleLatticeWriter::Worker::~Worker() {
}

#include "doc_lattice_reader.hpp"

#include <cstdio>
#include <cstdlib>

#include <boost/assign/list_of.hpp>

#include "logging.hpp"


std::string DocLatticeReader::getFormatName() {
    return "Doc";
}

std::string DocLatticeReader::doInfo() {
    return "Doc reader";
}

DocLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* DocLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new DocLatticeReader();
}

boost::program_options::options_description DocLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string DocLatticeReader::Factory::doGetName() {
    return "doc-reader";
}

boost::filesystem::path DocLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

DocLatticeReader::Worker::Worker(
    DocLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("doc-reader")))
{ }

void DocLatticeReader::Worker::doRun() {
// SET_LOGGING_LEVEL("DEBUG");
    char * tmpFileIn = tempnam(NULL, "psii_");
    std::ofstream s1(tmpFileIn);
    s1 << inputStream_.rdbuf();
    std::string tmpFileInName(tmpFileIn);
    std::string command("antiword " + tmpFileInName);
    // std::string command("antiword /tmp/psii_psb5XX");
DEBUG(command);
    system(command.c_str());

    std::remove(tmpFileIn);
    free(tmpFileIn);
/*
    char * tmpFileOut = tempnam(NULL, "psio_");
    try {
        std::ifstream s2(tmpFileOut);
        std::string line;
        while (std::getline(s2, line)) {
            appendParagraphToLattice_(line);
            lattice_.appendString("\n");
        }
        std::remove(tmpFileOut);
        free(tmpFileOut);
    } catch (...) {
        std::remove(tmpFileOut);
        free(tmpFileOut);
    }
    */
}

void DocLatticeReader::Worker::appendParagraphToLattice_(std::string paragraph) {
    Lattice::VertexDescriptor prevEnd = lattice_.getLastVertex();
    lattice_.appendStringWithSymbols(paragraph);
    Lattice::VertexDescriptor nowEnd = lattice_.getLastVertex();

    AnnotationItem item("FRAG", paragraph);
    lattice_.addEdge(prevEnd, nowEnd,
                     item, textTags_);

}

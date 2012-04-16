#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>

#include "guessing_reader.hpp"

#include "txt_lattice_reader.hpp"
#include "utt_lattice_reader.hpp"
#include "psi_lattice_reader.hpp"
#include "apertium_lattice_reader.hpp"
#include "nkjp_lattice_reader.hpp"
#if HAVE_POPPLER
#include "pdf_lattice_reader.hpp"
#endif
#if HAVE_DJVULIBRE
#include "djvu_lattice_reader.hpp"
#endif
#if USE_DOC_READER
#include "doc_lattice_reader.hpp"
#endif

const int GuessingReader::DEFAULT_BLOCK_SIZE = 1024;

std::string GuessingReader::getFormatName() {
    return "Guessing";
}

std::string GuessingReader::doInfo() {
    return "Guessing reader";
}

GuessingReader::GuessingReader() : blockSize_(DEFAULT_BLOCK_SIZE) { }
GuessingReader::GuessingReader(int blockSize) : blockSize_(blockSize) { }

GuessingReader::Factory::~Factory() { }

LatticeReader<std::istream>* GuessingReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& options) {

    return new GuessingReader(options["block-size"].as<int>());
}

boost::program_options::options_description GuessingReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("block-size", boost::program_options::value<int>()->default_value(DEFAULT_BLOCK_SIZE),
            "the size of the input data used to determine the format")
        ;

    return optionsDescription;
}

std::string GuessingReader::Factory::doGetName() const {
    return "guessing-reader";
}

boost::filesystem::path GuessingReader::Factory::doGetFile() const {
    return __FILE__;
}

GuessingReader::Worker::Worker(GuessingReader& processor,
                               std::istream& inputStream,
                               Lattice& lattice) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    fragTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("guessing-reader"))) {
}

void GuessingReader::Worker::doRun() {
    std::string input;
    std::string line;

    while (std::getline(inputStream_, line)) {
        if (boost::algorithm::trim_copy(line).empty()) {
            continue;
        }
        input += line;
    }

    std::cout << input << std::endl;
}

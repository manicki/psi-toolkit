#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

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

std::map<std::string, boost::shared_ptr<LatticeReaderFactory<std::istream> > >
    GuessingReader::mimeTypeToReaderMap_ = boost::assign::map_list_of
        ("a", new TxtLatticeReader::Factory());

std::string GuessingReader::getFormatName() {
    return "Guessing";
}

std::string GuessingReader::doInfo() {
    return "Guessing reader";
}

GuessingReader::GuessingReader() : blockSize_(DEFAULT_BLOCK_SIZE) { }
GuessingReader::GuessingReader(int blockSize) : blockSize_(blockSize) { }

std::string GuessingReader::guessFileMimeType(std::istream& inputStream) {
    char buffer[blockSize_];
    inputStream.read(buffer, blockSize_);
    std::string blockData(buffer);

    //TODO:: do something with blockData
/*
    int lastReadBit = blockSize_;
    for (int i = 0; i < blockSize_; i++) {
        if (buffer[i] == std::istream::eofbit) {
            lastReadBit = i;
            break;
        }
    }
*/
    for (int i = blockSize_; i > 0; i--) {
        inputStream.putback(buffer[i - 1]);
    }

    return std::string("a");
}

LatticeReader<std::istream>* GuessingReader::getLatticeReaderForMimeType(std::string mime) {
    boost::program_options::variables_map options;

    return mimeTypeToReaderMap_[mime]->createLatticeReader(options);
}

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
    processor_(processor) { }

void GuessingReader::Worker::doRun() {
    std::string mime = processor_.guessFileMimeType(inputStream_);
    DEBUG("guessed mime type: " << mime);

    LatticeReader<std::istream>* reader = processor_.getLatticeReaderForMimeType(mime);

    if (reader != NULL) {
        DEBUG("guessed reader for: " << reader->getFormatName());
        reader->readIntoLattice(inputStream_, lattice_);
    }
    else {
        ERROR("The unknown reader for the guessed mime type!");
    }
}

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
    GuessingReader::fileTypeToReaderMap_ = boost::assign::map_list_of
        //("html", new ApertiumLatticeReader::Factory())
        ("txt", new TxtLatticeReader::Factory());

std::string GuessingReader::getFormatName() {
    return "Guessing";
}

std::string GuessingReader::doInfo() {
    return "Guessing reader";
}

GuessingReader::GuessingReader() : blockSize_(DEFAULT_BLOCK_SIZE) { }
GuessingReader::GuessingReader(int blockSize) : blockSize_(blockSize) { }

std::string GuessingReader::guessFileType(std::istream& input) {
    std::string data = getStartingDataBlockWithoutTouchingIStream_(input);

    return fileRecognizer_.recognizeFileExtension(data);
}

std::string GuessingReader::getStartingDataBlockWithoutTouchingIStream_(std::istream& stream) {
    char buffer[blockSize_];
    stream.read(buffer, blockSize_);

    DEBUG("buffer: " << buffer);

    //stream.seekg(0, std::istream::end);
    //int length = stream.tellg();
    //stream.seekg(0, std::istream::beg);

    //DEBUG("length of data within stream: " << length);

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
        stream.putback(buffer[i - 1]);
    }

    //DEBUG("buffer: " << buffer);
    return std::string(buffer);
}

LatticeReader<std::istream>* GuessingReader::getLatticeReader(std::string type) {
    boost::program_options::variables_map options;

    std::map<std::string, boost::shared_ptr<LatticeReaderFactory<std::istream> > >
        ::iterator foundReader = fileTypeToReaderMap_.find(type);

    if (foundReader != fileTypeToReaderMap_.end()) {
        return foundReader->second->createLatticeReader(options);
    }
    return NULL;
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
    std::string filetype = processor_.guessFileType(inputStream_);
    DEBUG("guessed file type: " << filetype);

    LatticeReader<std::istream>* reader = processor_.getLatticeReader(filetype);

    if (reader != NULL) {
        DEBUG("guessed reader for: " << reader->getFormatName());
        reader->readIntoLattice(inputStream_, lattice_);
    }
    else {
        ERROR("The unknown reader for the guessed file type!");
    }
}

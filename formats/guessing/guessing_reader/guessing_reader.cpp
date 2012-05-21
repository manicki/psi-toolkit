#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/program_options/parsers.hpp>

#include "guessing_reader.hpp"

#include "txt_lattice_reader.hpp"
//#include "apertium_lattice_reader.hpp"
#if HAVE_POPPLER
#include "pdf_lattice_reader.hpp"
#endif
#if HAVE_DJVULIBRE
#include "djvu_lattice_reader.hpp"
#endif
#if USE_DOC_READER
#include "doc_lattice_reader.hpp"
#endif

const int GuessingReader::DEFAULT_BLOCK_SIZE = 32;

std::map<std::string, GuessingReader::PointerToReader> GuessingReader::fileTypeToReaderMap_ =
    boost::assign::map_list_of
        ("txt", PointerToReader(new TxtLatticeReader::Factory()))
        //("html", PointerToReader(new ApertiumLatticeReader::Factory()))
#if HAVE_POPPLER
        ("pdf", PointerToReader(new PDFLatticeReader::Factory()))
#endif
#if HAVE_DJVULIBRE
        ("djvu", PointerToReader(new DjVuLatticeReader::Factory()))
#endif
#if USE_DOC_READER
        ("doc", PointerToReader(new DocLatticeReader::Factory()))
#endif
        ;

std::map<std::string, std::string> GuessingReader::fileTypeToReaderOptionsMap_ =
    boost::assign::map_list_of
        ("html", "--format html")
        ;

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

    std::string filetype = fileRecognizer_.recognizeFileExtension(data);
    if (formatRecognizer_.isHandledFiletype(filetype)) {
        filetype = formatRecognizer_.recognize(data, filetype);
    }

    return filetype;
}

std::string GuessingReader::getStartingDataBlockWithoutTouchingIStream_(std::istream& stream) {
    char buffer[blockSize_];
    stream.read(buffer, blockSize_);

    // it is necessary to remove eofbit flag in the case of when the blockSize_
    // is greather than the total length of input
    stream.clear();

    int lastReadable = (int)stream.gcount();
    DEBUG("read bits for input type recognition (" << lastReadable << " bits): " << buffer);

    for (int i = lastReadable; i > 0; i--) {
        stream.putback(buffer[i - 1]);
    }

    return std::string(buffer, lastReadable);
}

LatticeReader<std::istream>* GuessingReader::getLatticeReader(std::string type) {
    boost::program_options::variables_map options;

    std::map<std::string, PointerToReader>::iterator foundReader
        = fileTypeToReaderMap_.find(type);
    std::map<std::string, std::string>::iterator foundOptions
        = fileTypeToReaderOptionsMap_.find(type);

    if (foundReader != fileTypeToReaderMap_.end()) {
        DEBUG("guessed reader for: " << type);

        if (foundOptions != fileTypeToReaderOptionsMap_.end()) {
            DEBUG("with options: " << foundOptions->second);

            std::vector<std::string> optionsArgv;
            boost::split(optionsArgv, foundOptions->second, boost::is_any_of(" "));

            int argc;
            boost::scoped_array<char*> argv(new char* [optionsArgv.size() + 2]);

            for (argc = 1; argc < (int)optionsArgv.size(); argc++) {
                argv[argc] = const_cast<char *>(optionsArgv[argc].c_str());
            }
            argv[++argc] = 0;

            boost::program_options::store(
                boost::program_options::parse_command_line(
                    argc, argv.get(), foundReader->second->optionsHandled()),
                options);

            boost::program_options::notify(options);
        }

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
    LatticeReader<std::istream>* reader = processor_.getLatticeReader(filetype);

    if (reader != NULL) {
        reader->readIntoLattice(inputStream_, lattice_);
    }
    else {
        ERROR("The unknown reader for the guessed file type!");
    }
}

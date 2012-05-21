#ifndef GUESSING_READER_HDR
#define GUESSING_READER_HDR

#include <boost/scoped_array.hpp>

#include <string>

#include "stream_lattice_reader.hpp"
#include "lattice_reader_factory.hpp"
#include "file_recognizer.hpp"

#include "logging.hpp"

class GuessingReader : public StreamLatticeReader {
public:

    GuessingReader();
    GuessingReader(int blockSize);

    std::string getFormatName();

    std::string guessFileType(std::istream&);
    LatticeReader<std::istream>* getLatticeReader(std::string);

    class Factory : public LatticeReaderFactory<std::istream> {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader<std::istream>* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;
    };

private:

    virtual std::string doInfo();

    class Worker : public ReaderWorker<std::istream> {
    public:
        Worker(GuessingReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        GuessingReader& processor_;
    };

    virtual ReaderWorker<std::istream>* doCreateReaderWorker(
        std::istream& inputStream, Lattice& lattice) {

        return new Worker(*this, inputStream, lattice);
    }

    const static int DEFAULT_BLOCK_SIZE;
    int blockSize_;

    typedef boost::shared_ptr<LatticeReaderFactory<std::istream> > PointerToReader;
    static std::map<std::string, PointerToReader> fileTypeToReaderMap_;
    static std::map<std::string, std::string> fileTypeToReaderOptionsMap_;

    std::string getStartingDataBlockWithoutTouchingIStream_(std::istream&);
    FileRecognizer fileRecognizer_;
};

#endif

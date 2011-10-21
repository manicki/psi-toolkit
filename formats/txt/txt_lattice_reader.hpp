#ifndef TXT_LATTICE_READER_HDR
#define TXT_LATTICE_READER_HDR


#include "stream_lattice_reader.hpp"
#include "lattice_reader_factory.hpp"

class TxtLatticeReader : public StreamLatticeReader {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeReaderFactory {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
    };

private:
    virtual std::string doInfo();

    class Worker : public ReaderWorker {
    public:
        Worker(TxtLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        void appendParagraphToLattice_(std::string paragraph);

        Processor& processor_;

        LayerTagCollection textTags_;
    };

    virtual ReaderWorker* doCreateReaderWorker(std::istream& inputStream, Lattice& lattice) {
        return new Worker(*this, inputStream, lattice);
    }
};

#endif

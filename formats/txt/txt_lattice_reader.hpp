#ifndef TXT_LATTICE_READER_HDR
#define TXT_LATTICE_READER_HDR


#include "lattice_reader.hpp"
#include "lattice_reader_factory.hpp"

class TxtLatticeReader : public LatticeReader {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeReaderFactory {
    private:
        virtual LatticeReader* doCreateLatticeReader(
            boost::program_options::variables_map options);

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
    };

    virtual ReaderWorker* doCreateReaderWorker(std::istream& inputStream, Lattice& lattice) {
        return new Worker(*this, inputStream, lattice);
    }
};

#endif

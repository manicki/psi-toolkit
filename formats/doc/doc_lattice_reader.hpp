#ifndef DOC_LATTICE_READER_HDR
#define DOC_LATTICE_READER_HDR


#include "stream_lattice_reader.hpp"
#include "lattice_reader_factory.hpp"
#include "plugin/antiword_adapter_interface.hpp"


class DocLatticeReader : public StreamLatticeReader {

public:
    DocLatticeReader();
    ~DocLatticeReader();

    virtual std::string getFormatName();

    AntiwordAdapterInterface * getAdapter();
    bool isActive();

    class Factory : public LatticeReaderFactory<std::istream> {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader<std::istream>* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;

        virtual bool doCheckRequirements(
            const boost::program_options::variables_map& options,
            std::ostream & message
        ) const;
    };

private:
    virtual std::string doInfo();

    class Worker : public ReaderWorker<std::istream> {
    public:
        Worker(DocLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        DocLatticeReader& processor_;
        LayerTagCollection textTags_;
    };

    virtual ReaderWorker<std::istream>* doCreateReaderWorker(
        std::istream& inputStream, Lattice& lattice) {

        return new Worker(*this, inputStream, lattice);
    }

    AntiwordAdapterInterface * adapter_;
};


#endif

#ifndef DOT_LATTICE_WRITER_HDR
#define DOT_LATTICE_WRITER_HDR

#include <boost/foreach.hpp>

#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "aligning_writer_worker.hpp"
#include "psi_quoter.hpp"

class DotLatticeWriter : public LatticeWriter<std::ostream> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<std::ostream> {
    private:
        virtual LatticeWriter<std::ostream>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
        virtual boost::filesystem::path doGetFile();
    };


private:
    virtual std::string doInfo();

    class Worker : public AligningWriterWorker {
    public:
        Worker(DotLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        DotLatticeWriter& processor_;
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

};

#endif

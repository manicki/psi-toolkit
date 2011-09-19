#ifndef PSI_LATTICE_WRITER_HDR
#define PSI_LATTICE_WRITER_HDR

#include <boost/foreach.hpp>

#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "psi_quoter.hpp"

class PsiLatticeWriter : public LatticeWriter {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory {
    private:
        virtual LatticeWriter* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
    };


private:
    virtual std::string doInfo();

    class Worker : public WriterWorker {
    public:
        Worker(PsiLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        PsiLatticeWriter& processor_;
    };

    virtual WriterWorker* doCreateWriterWorker(std::ostream& outputStream, Lattice& lattice) {
        return new Worker(*this, outputStream, lattice);
    }

};

#endif

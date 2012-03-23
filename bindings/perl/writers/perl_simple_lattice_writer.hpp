#ifndef PERL_SIMPLE_LATTICE_WRITER_HDR
#define PERL_SIMPLE_LATTICE_WRITER_HDR

#include "config.hpp"

#if HAVE_PERL_BINDINGS

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <string>

#include "lattice_iter_writer.hpp"
#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "writer_worker.hpp"
#include "psi_quoter.hpp"
#include "perl_simple_lattice_writer_output_iterator.hpp"

#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

class PerlSimpleLatticeWriter : public LatticeWriter<Sink> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<Sink> {
    private:
        virtual LatticeWriter<Sink> * doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

    class Factory : public AbstractSimpleDataLatticeWriter::Factory {
    protected:
        virtual std::string doGetName();

        virtual boost::filesystem::path doGetFile();

        virtual LatticeWriter<AbstractSimpleDataLatticeWriterOutput>* createLatticeWriter(
           bool linear,
           bool noAlts,
           bool withBlank,
           std::string basicTag,
           std::set<std::string> higherOrderTags,
           bool withArgs );
    };

    PerlSimpleLatticeWriter(
        bool linear,
        bool noAlts,
        bool withBlank,
        std::string basicTag,
        std::set<std::string> higherOrderTags,
        bool withArgs
                            ) : AbstractSimpleDataLatticeWriter(
                                  linear,
                                  noAlts,
                                  withBlank,
                                  basicTag,
                                  higherOrderTags,
                                  withArgs
                              )
    { }

protected:
    virtual std::string doInfo();

    class Worker : public WriterWorker<Sink> {
    public:
        Worker(PerlSimpleLatticeWriter& processor,
               Sink & output,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        PerlSimpleLatticeWriter& processor_;
    };

    virtual WriterWorker<Sink> * doCreateWriterWorker(
        Sink & output, Lattice& lattice) {

        return new Worker(*this, output, lattice);
    }

    bool linear_;
    bool noAlts_;
    bool withBlank_;
    std::string basicTag_;
    std::set<std::string> higherOrderTags_;
    bool withArgs_;
};

#endif

#endif

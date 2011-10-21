#ifndef PERL_SIMPLE_LATTICE_WRITER_HDR
#define PERL_SIMPLE_LATTICE_WRITER_HDR

#include <boost/foreach.hpp>
#include <string>

#include "lattice_iter_writer.hpp"
#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "writer_worker.hpp"
#include "psi_quoter.hpp"

#include "perl_lattice_writer_output.hpp"
#include "perl_simple_lattice_writer_output_iterator.hpp"

class PerlSimpleLatticeWriter : public LatticeWriter<PerlLatticeWriterOutput> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<PerlLatticeWriterOutput> {
    private:
        virtual LatticeWriter<PerlLatticeWriterOutput>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
    };

    PerlSimpleLatticeWriter(
        bool linear,
        bool noAlts,
        bool withBlank,
        std::string basicTag
    ) :
        linear_(linear),
        noAlts_(noAlts),
        withBlank_(withBlank),
        basicTag_(basicTag)
    { }

    bool isLinear() const { return linear_; }
    bool isNoAlts() const { return noAlts_; }
    bool isWithBlank() const { return withBlank_; }
    std::string getBasicTag() const { return basicTag_; }

private:
    virtual std::string doInfo();

    class Worker : public WriterWorker<PerlLatticeWriterOutput> {
    public:
        Worker(PerlSimpleLatticeWriter& processor,
               PerlLatticeWriterOutput & output,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        PerlSimpleLatticeWriter& processor_;
    };

    virtual WriterWorker<PerlLatticeWriterOutput>* doCreateWriterWorker(PerlLatticeWriterOutput & output, Lattice& lattice) {
        return new Worker(*this, output, lattice);
    }

    bool linear_;
    bool noAlts_;
    bool withBlank_;
    std::string basicTag_;
};

#endif

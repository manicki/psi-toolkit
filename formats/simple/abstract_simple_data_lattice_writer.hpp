#ifndef ABSTRACT_SIMPLE_DATA_LATTICE_WRITER_HDR
#define ABSTRACT_SIMPLE_DATA_LATTICE_WRITER_HDR

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <string>

#include "lattice_iter_writer.hpp"
#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "writer_worker.hpp"
#include "psi_quoter.hpp"

#include "abstract_simple_data_lattice_writer_output.hpp"
#include "abstract_simple_data_lattice_writer_output_iterator.hpp"

class AbstractSimpleDataLatticeWriter : public LatticeWriter<AbstractSimpleDataLatticeWriterOutput> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<AbstractSimpleDataLatticeWriterOutput> {
    protected:
        virtual LatticeWriter<AbstractSimpleDataLatticeWriterOutput>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() = 0;

        virtual boost::filesystem::path doGetFile() = 0;

        virtual LatticeWriter<AbstractSimpleDataLatticeWriterOutput>* createLatticeWriter(
           bool linear,
           bool noAlts,
           bool withBlank,
           std::string basicTag,
           std::set<std::string> higherOrderTags,
           bool withArgs ) = 0;
    };

    AbstractSimpleDataLatticeWriter (
        bool linear,
        bool noAlts,
        bool withBlank,
        std::string basicTag,
        std::set<std::string> higherOrderTags,
        bool withArgs
    ) :
        linear_(linear),
        noAlts_(noAlts),
        withBlank_(withBlank),
        basicTag_(basicTag),
        higherOrderTags_(higherOrderTags),
        withArgs_(withArgs)
    { }

    bool isLinear() const {
        return linear_;
    }

    bool isNoAlts() const {
        return noAlts_;
    }

    bool isWithBlank() const {
        return withBlank_;
    }

    bool isWithArgs() const {
        return withArgs_;
    }

    std::string getBasicTag() const {
        return basicTag_;
    }

    std::set<std::string> getHigherOrderTags() const {
        return higherOrderTags_;
    }

protected:
    virtual std::string doInfo() = 0;

    class Worker : public WriterWorker<AbstractSimpleDataLatticeWriterOutput> {
    public:
        Worker(AbstractSimpleDataLatticeWriter& processor,
               AbstractSimpleDataLatticeWriterOutput & output,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        AbstractSimpleDataLatticeWriter& processor_;
    };

    virtual WriterWorker<AbstractSimpleDataLatticeWriterOutput>* doCreateWriterWorker(
        AbstractSimpleDataLatticeWriterOutput & output, Lattice& lattice) {

        return new Worker(*this, output, lattice);
    }

private:
    bool linear_;
    bool noAlts_;
    bool withBlank_;
    std::string basicTag_;
    std::set<std::string> higherOrderTags_;
    bool withArgs_;
};

#endif

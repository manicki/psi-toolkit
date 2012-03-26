#ifndef JSON_LATTICE_WRITER_HDR
#define JSON_LATTICE_WRITER_HDR

#include "config.hpp"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <string>

#include "lattice_iter_writer.hpp"
#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "writer_worker.hpp"
#include "psi_quoter.hpp"
#include "json_lattice_writer_output_iterator.hpp"

class JSONLatticeWriter : public LatticeWriter<std::ostream> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<std::ostream> {
    private:
        virtual LatticeWriter<std::ostream> * doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();

        virtual boost::filesystem::path doGetFile();
    };

    JSONLatticeWriter(
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

private:
    virtual std::string doInfo();

    class Worker : public WriterWorker<std::ostream> {
    public:
        Worker(JSONLatticeWriter& processor,
               std::ostream & output,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        JSONLatticeWriter& processor_;
    };

    virtual WriterWorker<std::ostream> * doCreateWriterWorker(
        std::ostream & output, Lattice& lattice) {

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

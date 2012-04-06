#ifndef SIMPLE_LATTICE_WRITER_HDR
#define SIMPLE_LATTICE_WRITER_HDR

#include <map>

#include <boost/foreach.hpp>

#include "lattice_iter_writer.hpp"
#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "aligning_writer_worker.hpp"
#include "psi_quoter.hpp"
#include "simple_lattice_writer_stream_output_iterator.hpp"

class SimpleLatticeWriter : public LatticeWriter<std::ostream> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<std::ostream> {
    private:
        virtual LatticeWriter<std::ostream>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
        virtual boost::filesystem::path doGetFile() const;
    };

    SimpleLatticeWriter(
        bool linear,
        bool noAlts,
        bool withBlank,
        std::string basicTag,
        std::string basicTagSeparator,
        std::string altSeparator,
        std::map<std::string, std::string> tagsSeparators
    ) :
        linear_(linear),
        noAlts_(noAlts),
        withBlank_(withBlank),
        basicTag_(basicTag),
        basicTagSeparator_(basicTagSeparator),
        altSeparator_(altSeparator),
        tagsSeparators_(tagsSeparators)
    { }

    bool isLinear() const { return linear_; }
    bool isNoAlts() const { return noAlts_; }
    bool isWithBlank() const { return withBlank_; }
    std::string getBasicTag() const { return basicTag_; }
    std::string getBasicTagSeparator() const { return basicTagSeparator_; }
    std::string getAltSeparator() const { return altSeparator_; }
    std::map<std::string, std::string> getTagsSeparatorsMap() const { return tagsSeparators_; }

    bool isHandledTag(std::string tagName) {
        return tagsSeparators_.find(tagName) != tagsSeparators_.end();
    }

    std::string getTagSeparator(std::string tagName) {
        return tagsSeparators_[tagName];
    }


private:
    virtual std::string doInfo();

    class Worker : public AligningWriterWorker {
    public:
        Worker(SimpleLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        SimpleLatticeWriter& processor_;
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

    bool linear_;
    bool noAlts_;
    bool withBlank_;
    std::string basicTag_;
    std::string basicTagSeparator_;
    std::string altSeparator_;
    std::map<std::string, std::string> tagsSeparators_;

};

#endif

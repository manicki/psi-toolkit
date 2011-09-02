#ifndef SIMPLE_LATTICE_WRITER_HDR
#define SIMPLE_LATTICE_WRITER_HDR

#include <map>

#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"

class SimpleLatticeWriter : public LatticeWriter {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory {
    private:
        virtual LatticeWriter* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
    };

    SimpleLatticeWriter(
        bool linear,
        bool noAlts,
        std::string basicTag,
        std::string basicTagSeparator,
        std::string altSeparator,
        std::map<std::string, std::string> tagsSeparators
    ) :
        linear_(linear),
        noAlts_(noAlts),
        basicTag_(basicTag),
        basicTagSeparator_(basicTagSeparator),
        altSeparator_(altSeparator),
        tagsSeparators_(tagsSeparators)
    { }

    bool isLinear() const { return linear_; }
    bool isNoAlts() const { return noAlts_; }
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

    class Worker : public WriterWorker {
    public:
        Worker(SimpleLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        SimpleLatticeWriter& processor_;
    };

    virtual WriterWorker* doCreateWriterWorker(std::ostream& outputStream, Lattice& lattice) {
        return new Worker(*this, outputStream, lattice);
    }

    bool linear_;
    bool noAlts_;
    std::string basicTag_;
    std::string basicTagSeparator_;
    std::string altSeparator_;
    std::map<std::string, std::string> tagsSeparators_;

};

#endif

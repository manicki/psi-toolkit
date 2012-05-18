#ifndef BRACKETING_LATTICE_WRITER_HDR
#define BRACKETING_LATTICE_WRITER_HDR

#include <set>

#include "aligning_writer_worker.hpp"
#include "bracketing_lattice_writer_stream_output_iterator.hpp"
#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"


class BracketingLatticeWriter : public LatticeWriter<std::ostream> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<std::ostream> {
    private:
        virtual LatticeWriter<std::ostream>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;
    };

    BracketingLatticeWriter(
        std::string openingBracket,
        std::string closingBracket,
        std::string tagSeparator,
        std::vector<std::string> showOnlyTags,
        std::vector<std::string> filter,
        std::string avPairsSeparator,
        std::string avSeparator,
        std::vector<std::string> showAttributes
    ) :
        openingBracket_(openingBracket),
        closingBracket_(closingBracket),
        tagSeparator_(tagSeparator),
        showOnlyTags_(showOnlyTags.begin(), showOnlyTags.end()),
        filter_(filter),
        avPairsSeparator_(avPairsSeparator),
        avSeparator_(avSeparator),
        showAttributes_(showAttributes.begin(), showAttributes.end())
    { }

    std::string getOpeningBracket() const { return openingBracket_; }
    std::string getClosingBracket() const { return closingBracket_; }
    std::string getTagSeparator() const { return tagSeparator_; }
    std::vector<std::string> getFilter() const { return filter_; }
    std::string getAVPairsSeparator() const { return avPairsSeparator_; }
    std::string getAVSeparator() const { return avSeparator_; }

    std::set<std::string> intersectOnlyTags(std::set<std::string> tags);
    std::map<std::string, std::string> filterAttributes(std::map<std::string, std::string> avMap);

private:
    virtual std::string doInfo();

    class Worker : public AligningWriterWorker {
    public:
        Worker(BracketingLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        BracketingLatticeWriter& processor_;
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

    std::string openingBracket_;
    std::string closingBracket_;
    std::string tagSeparator_;
    std::set<std::string> showOnlyTags_;
    std::vector<std::string> filter_;
    std::string avPairsSeparator_;
    std::string avSeparator_;
    std::set<std::string> showAttributes_;

};


#endif

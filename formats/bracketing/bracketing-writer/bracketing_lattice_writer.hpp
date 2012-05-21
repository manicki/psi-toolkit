#ifndef BRACKETING_LATTICE_WRITER_HDR
#define BRACKETING_LATTICE_WRITER_HDR

#include <set>
#include <stack>

#include "active_elements_printer.hpp"
#include "aligning_writer_worker.hpp"
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
    );

    std::string getOpeningBracket() const { return openingBracket_; }
    std::string getClosingBracket() const { return closingBracket_; }
    std::string getTagSeparator() const { return tagSeparator_; }
    std::vector<std::string> getFilter() const { return filter_; }
    std::string getAVPairsSeparator() const { return avPairsSeparator_; }
    std::string getAVSeparator() const { return avSeparator_; }

    std::set<std::string> intersectOnlyTags(std::set<std::string> tags);
    std::map<std::string, std::string> filterAttributes(std::map<std::string, std::string> avMap);

    bool isActive_T() const { return active_T_; }
    bool isActive_c() const { return active_c_; }
    bool isActive_t() const { return active_t_; }
    bool isActive_A() const { return active_A_; }
    bool isActive_s() const { return active_s_; }

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

        EdgeData getEdgeData_(Lattice::EdgeDescriptor edge);

        void collapse_(std::set<EdgeData> & edgeDataSet);
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

    bool active_T_;
    bool active_c_;
    bool active_t_;
    bool active_A_;
    bool active_s_;

    void setActiveElements_(std::string pattern);

};


#endif

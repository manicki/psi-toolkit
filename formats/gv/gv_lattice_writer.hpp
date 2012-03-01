#ifndef GV_LATTICE_WRITER_HDR
#define GV_LATTICE_WRITER_HDR

#include <set>
#include <string>
#include <list>

#include <boost/foreach.hpp>

#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "aligning_writer_worker.hpp"
#include "psi_quoter.hpp"

class GVLatticeWriter : public LatticeWriter<std::ostream> {

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

    GVLatticeWriter(
        bool showTags,
        bool color,
        std::set<std::string> filter,
        std::string outputFormat,
        std::string outputFile
    ) :
        showTags_(showTags),
        color_(color),
        filter_(filter),
        outputFormat_(outputFormat),
        outputFile_(outputFile)
    { }

    bool isShowTags() const { return showTags_; }
    bool isColor() const { return color_; }
    bool isInFilter(std::string tag) { return filter_.empty() || filter_.count(tag); }

    bool areSomeInFilter(std::list<std::string> & tags) {
        if (filter_.empty()) return true;
        BOOST_FOREACH(std::string tag, tags) {
            if (filter_.count(tag)) return true;
        }
        return false;
    }

    std::string getOutputFormat() const { return outputFormat_; }
    std::string getOutputFile() const { return outputFile_; }

private:
    virtual std::string doInfo();

    class Worker : public AligningWriterWorker {
    public:
        Worker(GVLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        GVLatticeWriter& processor_;
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

    bool showTags_;
    bool color_;
    std::set<std::string> filter_;
    std::string outputFormat_;
    std::string outputFile_;

};

#endif

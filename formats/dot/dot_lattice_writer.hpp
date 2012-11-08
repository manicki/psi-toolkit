#ifndef DOT_LATTICE_WRITER_HDR
#define DOT_LATTICE_WRITER_HDR

#include <set>
#include <string>
#include <list>

#include <boost/foreach.hpp>

#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "aligning_writer_worker.hpp"
#include "psi_quoter.hpp"

class DotLatticeWriter : public LatticeWriter<std::ostream> {

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

    DotLatticeWriter(
        bool showTags,
        bool color,
        std::set<std::string> filter,
        bool tree,
        bool align,
        bool showSymbolEdges
    ) :
        showTags_(showTags),
        color_(color),
        filter_(filter),
        tree_(tree),
        align_(align),
        showSymbolEdges_(showSymbolEdges)
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

    bool isTree() const { return tree_; }
    bool isAlign() const { return align_; }
    bool isShowSymbolEdges() const { return showSymbolEdges_; }

private:
    virtual std::string doInfo();

    class Worker : public AligningWriterWorker {
    public:
        Worker(DotLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        DotLatticeWriter& processor_;
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

    bool showTags_;
    bool color_;
    std::set<std::string> filter_;
    bool tree_;
    bool align_;
    bool showSymbolEdges_;

};

#endif

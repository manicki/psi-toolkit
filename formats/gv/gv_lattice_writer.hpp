#ifndef GV_LATTICE_WRITER_HDR
#define GV_LATTICE_WRITER_HDR

#include <set>
#include <string>
#include <list>

#include <boost/foreach.hpp>

// Fixes warnings "... not defined" from GraphViz include files

#ifndef _BLD_cdt
#define _BLD_cdt 0
#endif

#ifndef _DLL_BLD
#define _DLL_BLD 0
#endif

#ifndef _dll_import
#define _dll_import 0
#endif

#ifndef _PACKAGE_ast
#define _PACKAGE_ast 0
#endif

#include <gvc.h>

#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "aligning_writer_worker.hpp"
#include "plugin/graphviz_adapter_interface.hpp"
#include "psi_quoter.hpp"


class GVLatticeWriter : public LatticeWriter<std::ostream> {

public:
    GVLatticeWriter();

    GVLatticeWriter(
        bool showTags,
        bool color,
        std::set<std::string> filter,
        std::string outputFormat,
        bool tree
    );

    ~GVLatticeWriter();

    virtual std::string getFormatName();

    GraphvizAdapterInterface * getAdapter();
    bool isActive();

    class Factory : public LatticeWriterFactory<std::ostream> {
    private:
        virtual LatticeWriter<std::ostream>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;
    };

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
    bool isTree() const { return tree_; }

private:
    void init_();

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
        // std::set<std::string> allowedFormats_;
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

    bool showTags_;
    bool color_;
    std::set<std::string> filter_;
    std::string outputFormat_;
    bool tree_;

    GraphvizAdapterInterface * adapter_;

};


#endif

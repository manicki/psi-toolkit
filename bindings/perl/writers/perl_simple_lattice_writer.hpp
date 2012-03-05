#ifndef PERL_SIMPLE_LATTICE_WRITER_HDR
#define PERL_SIMPLE_LATTICE_WRITER_HDR

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <string>

#include "abstract_simple_data_lattice_writer.hpp"

#include "perl_lattice_writer_output.hpp"
#include "perl_simple_lattice_writer_output_iterator.hpp"

class PerlSimpleLatticeWriter : public AbstractSimpleDataLatticeWriter {

public:

    class Factory : public AbstractSimpleDataLatticeWriter::Factory {
    protected:
        virtual std::string doGetName();

        virtual boost::filesystem::path doGetFile();

        virtual LatticeWriter<AbstractSimpleDataLatticeWriterOutput>* createLatticeWriter(
           bool linear,
           bool noAlts,
           bool withBlank,
           std::string basicTag,
           std::set<std::string> higherOrderTags,
           bool withArgs );
    };

    PerlSimpleLatticeWriter(
        bool linear,
        bool noAlts,
        bool withBlank,
        std::string basicTag,
        std::set<std::string> higherOrderTags,
        bool withArgs
                            ) : AbstractSimpleDataLatticeWriter(
                                  linear,
                                  noAlts,
                                  withBlank,
                                  basicTag,
                                  higherOrderTags,
                                  withArgs
                              )
    { }

protected:
    virtual std::string doInfo();
};

#endif

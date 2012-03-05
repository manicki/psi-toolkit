#include "perl_simple_lattice_writer.hpp"

#include "logging.hpp"

std::string PerlSimpleLatticeWriter::Factory::doGetName() {
    return "perl-simple-writer";
}

boost::filesystem::path PerlSimpleLatticeWriter::Factory::doGetFile() {
    return __FILE__;
}

LatticeWriter<AbstractSimpleDataLatticeWriterOutput>* PerlSimpleLatticeWriter::Factory::createLatticeWriter(
           bool linear,
           bool noAlts,
           bool withBlank,
           std::string basicTag,
           std::set<std::string> higherOrderTags,
           bool withArgs ) {

    return new PerlSimpleLatticeWriter(
           linear,
           noAlts,
           withBlank,
           basicTag,
           higherOrderTags,
           withArgs );
}

std::string PerlSimpleLatticeWriter::doInfo() {
    return "Perl simple writer";
}


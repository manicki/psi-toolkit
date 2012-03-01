#include "tests.hpp"

#include <fstream>
#include <set>
#include <string>

#include <boost/scoped_ptr.hpp>

#include "lattice_preparators.hpp"
#include "gv_lattice_writer.hpp"


BOOST_AUTO_TEST_SUITE( gv_lattice_writer )


BOOST_AUTO_TEST_CASE( gv_lattice_writer_simple ) {

    Lattice lattice;
    lattice_preparators::prepareSimpleLattice(lattice);

    std::set<std::string> filter;

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new GVLatticeWriter(
        true, // show tags
        false, // color
        filter, // filter
        "dot", // output format
        "", // output file name
        true // use outputStream
    ));

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/gv/t/files/simple.dot");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_CASE( gv_lattice_writer_advanced ) {

    Lattice lattice;
    lattice_preparators::prepareAdvancedLattice(lattice);

    std::set<std::string> filter;

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new GVLatticeWriter(
        true, // show tags
        false, // color
        filter, // filter
        "dot", // output format
        "", // output file name
        true // use outputStream
    ));

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/gv/t/files/advanced.dot");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_SUITE_END()

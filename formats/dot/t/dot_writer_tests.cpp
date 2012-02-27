#include "tests.hpp"

#include <fstream>

#include <boost/scoped_ptr.hpp>

#include "lattice_preparators.hpp"
#include "dot_lattice_writer.hpp"


BOOST_AUTO_TEST_SUITE( dot_lattice_writer )


BOOST_AUTO_TEST_CASE( dot_lattice_writer ) {

    Lattice lattice;
    lattice_preparators::prepareSimpleLattice(lattice);

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new DotLatticeWriter());

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/dot/t/files/simple.dot");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_SUITE_END()

#include "tests.hpp"

#include <boost/scoped_ptr.hpp>

#include "psi_lattice_reader.hpp"
#include "psi_lattice_writer.hpp"


BOOST_AUTO_TEST_SUITE( psi_lattice_reader )


BOOST_AUTO_TEST_CASE( psi_lattice_reader_reflexive ) {

    Lattice lattice("");

    boost::scoped_ptr<StreamLatticeReader> reader(new PsiLatticeReader());

    BOOST_CHECK_EQUAL(reader->getFormatName(), "PSI");
    BOOST_CHECK_EQUAL(reader->info(), "PSI reader");

    reader->readIntoLattice(ROOT_DIR "formats/psi/t/files/pl_sample_nocomments.txt", lattice);

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter());

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/psi/t/files/pl_sample_nocomments.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_CASE( psi_lattice_reader_corrupted ) {

    Lattice lattice("");

    boost::scoped_ptr<StreamLatticeReader> reader(new PsiLatticeReader());

    BOOST_CHECK_THROW(
        reader->readIntoLattice(ROOT_DIR "formats/psi/t/files/corrupted.txt", lattice),
        FileFormatException
    );

}


BOOST_AUTO_TEST_SUITE_END()

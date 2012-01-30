#include "tests.hpp"

#include <fstream>
#include <iostream>

#include <boost/scoped_ptr.hpp>

#include "lattice_iter_writer.hpp"
#include "simple_lattice_writer_stream_output_iterator.hpp"
#include "writers_tests_utils.hpp"

#include "config.hpp"


BOOST_AUTO_TEST_SUITE( lattice_iter_writer )


BOOST_AUTO_TEST_CASE( lattice_iter_writer ) {

    Lattice lattice;
    writers_tests_utils::prepareSimpleLattice(lattice);

    std::vector<std::string> handledTags;
    handledTags.push_back("token");

    std::ostringstream osstr;

    SimpleLatticeWriterStreamOutputIterator outputIterator(
        osstr,
        "|",
        ","
    );

    outputIterator.setSeparator("token", ";");

    boost::scoped_ptr<LatticeIterWriter> writer(new LatticeIterWriter(
        lattice,
        outputIterator,
        false, //linear
        false, //no-alts
        true, //with-blank
        "symbol", //basicTag
        handledTags
    ));

    writer->run();

    osstr << std::endl;

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/simple/t/files/simple_ala.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_SUITE_END()

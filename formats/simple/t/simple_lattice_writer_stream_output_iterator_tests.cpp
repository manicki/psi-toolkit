#include "tests.hpp"

#include <fstream>
#include <iostream>

#include <boost/scoped_ptr.hpp>

#include "lattice_iter_writer.hpp"
#include "lattice_preparators.hpp"
#include "simple_lattice_writer_stream_output_iterator.hpp"

#include "config.hpp"


BOOST_AUTO_TEST_SUITE( simple_lattice_writer_stream_output_iterator )


BOOST_AUTO_TEST_CASE( simple_lattice_writer_stream_output_iterator ) {

    Lattice lattice;
    lattice_preparators::prepareSimpleLattice(lattice);

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


BOOST_AUTO_TEST_CASE( simple_lattice_writer_stream_output_iterator_advanced ) {

    Lattice lattice;
    lattice_preparators::prepareRegularLattice(lattice);

    std::vector<std::string> handledTags;
    handledTags.push_back("level1");
    handledTags.push_back("level2");

    std::ostringstream osstr;

    SimpleLatticeWriterStreamOutputIterator outputIterator(
        osstr,
        "|",
        ","
    );

    outputIterator.setSeparator("level1", ";");
    outputIterator.setSeparator("level2", "\n");

    boost::scoped_ptr<LatticeIterWriter> writer(new LatticeIterWriter(
        lattice,
        outputIterator,
        false, //linear
        false, //no-alts
        true, //with-blank
        "token", //basicTag
        handledTags
    ));

    writer->run();

    osstr << std::endl;

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/simple/t/files/regular.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_SUITE_END()

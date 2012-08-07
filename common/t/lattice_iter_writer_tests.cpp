#include "tests.hpp"

#include <fstream>
#include <iostream>

#include <boost/scoped_ptr.hpp>

#include "annotation_item_manager.hpp"
#include "lattice.hpp"
#include "lattice_iter_writer.hpp"
#include "lattice_preparators.hpp"
#include "literal_test_output_iterator.hpp"

#include "config.hpp"


BOOST_AUTO_TEST_SUITE( lattice_iter_writer )


BOOST_AUTO_TEST_CASE( lattice_iter_writer ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    lattice_preparators::prepareRegularLattice(lattice);

    std::vector<std::string> handledTags;
    handledTags.push_back("level1");
    handledTags.push_back("level2");

    std::ostringstream osstr;

    LiteralTestOutputIterator outputIterator(
        osstr
    );

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

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "common/t/files/iter_writer_regular_output.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_SUITE_END()

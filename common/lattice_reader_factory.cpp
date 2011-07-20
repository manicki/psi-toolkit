#include "lattice_reader_factory.hpp"

LatticeReader* LatticeReaderFactory::createLatticeReader(boost::program_options::variables_map options) {
    return dynamic_cast<LatticeReader*>(createProcessor(options));
}


LatticeReader* LatticeReaderFactory::doCreateProcessor(boost::program_options::variables_map options) {
    return doCreateLatticeReader(options);
}

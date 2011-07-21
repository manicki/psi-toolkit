#ifndef LATTICE_READER_FACTORY_HDR
#define LATTICE_READER_FACTORY_HDR

#include "processor_factory.hpp"
#include "lattice_reader.hpp"

class LatticeReaderFactory : public ProcessorFactory {

public:
    /**
     * Creates a lattice reader with the options specified.
     */
    LatticeReader* createLatticeReader(boost::program_options::variables_map options);

private:

    virtual LatticeReader* doCreateProcessor(boost::program_options::variables_map options);

    virtual LatticeReader* doCreateLatticeReader(boost::program_options::variables_map options) = 0;

};

#endif

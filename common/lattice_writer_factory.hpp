#ifndef LATTICE_WRITER_FACTORY_HDR
#define LATTICE_WRITER_FACTORY_HDR

#include "processor_factory.hpp"
#include "lattice_writer.hpp"

class LatticeWriterFactory : public ProcessorFactory {

public:
    /**
     * Creates a lattice writer with the options specified.
     */
    LatticeWriter* createLatticeWriter(const boost::program_options::variables_map& options);

    virtual ~LatticeWriterFactory();

private:

    virtual LatticeWriter* doCreateProcessor(const boost::program_options::variables_map& options);

    virtual LatticeWriter* doCreateLatticeWriter(const boost::program_options::variables_map& options) = 0;

};

#endif

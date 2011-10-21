#ifndef LATTICE_READER_FACTORY_HDR
#define LATTICE_READER_FACTORY_HDR

#include "processor_factory.hpp"
#include "lattice_reader.hpp"

template<typename Source>
class LatticeReaderFactory : public ProcessorFactory {

public:
    /**
     * Creates a lattice reader with the options specified.
     */
    LatticeReader<Source>* createLatticeReader(
        const boost::program_options::variables_map& options) {
        return dynamic_cast<LatticeReader<Source>*>(createProcessor(options));
    }

private:

    virtual LatticeReader<Source>* doCreateProcessor(
        const boost::program_options::variables_map& options) {
        return doCreateLatticeReader(options);
    }

    virtual LatticeReader<Source>* doCreateLatticeReader(
        const boost::program_options::variables_map& options) = 0;

    virtual std::string doGetContinuation(
        const boost::program_options::variables_map& /*options*/) const {
        return std::string("psi-writer");
    }

};

#endif

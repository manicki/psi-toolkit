#ifndef LATTICE_WRITER_FACTORY_HDR
#define LATTICE_WRITER_FACTORY_HDR

#include "processor_factory.hpp"
#include "lattice_writer.hpp"

template<typename Sink>
class LatticeWriterFactory : public ProcessorFactory {

public:
    /**
     * Creates a lattice writer with the options specified.
     */
    LatticeWriter<Sink>* createLatticeWriter(const boost::program_options::variables_map& options) {
        return dynamic_cast<LatticeWriter<Sink>*>(createProcessor(options));
    }

    virtual ~LatticeWriterFactory() {
    }

private:

    virtual LatticeWriter<Sink>* doCreateProcessor(const boost::program_options::variables_map& options) {
        return doCreateLatticeWriter(options);
    }

    virtual LatticeWriter<Sink>* doCreateLatticeWriter(const boost::program_options::variables_map& options) = 0;

    std::string doGetContinuation(
        const boost::program_options::variables_map& /*options*/) const {
        return std::string();
    }

};

#endif

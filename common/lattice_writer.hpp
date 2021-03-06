#ifndef LATTICE_WRITER_HDR
#define LATTICE_WRITER_HDR

#include <string>
#include <ostream>

#include <boost/scoped_ptr.hpp>

#include "lattice.hpp"
#include "writer_worker.hpp"
#include "processor.hpp"

/*!
  Lattice writer takes a lattice and writes its content to a sink (e.g. a stream) in some format.
*/
template<typename Sink>
class LatticeWriter : public Processor {

public:
    void writeLattice(Lattice& lattice, Sink& sink) {
        boost::scoped_ptr<WriterWorker<Sink> > worker(doCreateWriterWorker(sink, lattice));
        worker->run();
    }

    /**
     * Gets format name (eg. "UTT", "BIN" etc.)
     */
    virtual std::string getFormatName() = 0;

    virtual ~LatticeWriter() {
    }

private:
    virtual WriterWorker<Sink>* doCreateWriterWorker(Sink& sink, Lattice& lattice) = 0;

};

#endif

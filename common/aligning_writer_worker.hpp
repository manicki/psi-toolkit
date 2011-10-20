#ifndef ALIGNING_WRITER_WORKER
#define ALIGNING_WRITER_WORKER

#include "writer_worker.hpp"

class AligningWriterWorker : public WriterWorker {

public:
    AligningWriterWorker(std::ostream& outputStream, Lattice& lattice);

    virtual ~AligningWriterWorker();

protected:
    /**
     * Prints output to outputStream_ and updates cursor position.
     * Returns the new cursor position.
     */
    unsigned int alignOutput_(std::string output);

    /**
     * Prints output to outputStream_ and aligns cursor to the required position (pos) if possible.
     * Strings are aligned left.
     * Returns the real new cursor position.
     */
    unsigned int alignOutput_(std::string output, unsigned int pos, char padChar = ' ');

    /**
     * Prints newline to outputStream_ and resets cursor position.
     * Returns the new cursor position (zero).
     */
    unsigned int alignOutputNewline_();

private:
    unsigned int currentPos_;

};

#endif

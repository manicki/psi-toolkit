#ifndef STREAM_LATTICE_READER_HDR
#define STREAM_LATTICE_READER_HDR

#include "lattice_reader.hpp"

class StreamLatticeReader : public LatticeReader<std::istream> {

public:

    StreamLatticeReader();

    class CannotOpenFileException : public std::runtime_error {
    public:
        CannotOpenFileException() : std::runtime_error("cannot open a file") { }
    };

    /**
     * Reads from file `fileName` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(const std::string& fileName, Lattice& lattice);

};

#endif

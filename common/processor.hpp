#ifndef PROCESSOR_HDR
#define PROCESSOR_HDR

#include <string>

/*!
  Any module operating on a lattice is called a processor. Examples
  or processors are annotators, lattice readers and writers.
*/
class Processor {

public:
    /**
     * Returns some information about the processor and - possibly - its state
     * (e.g. its description, options used, some statistics).
     */
    std::string info();

public:
    virtual std::string doInfo() = 0;

};


#endif

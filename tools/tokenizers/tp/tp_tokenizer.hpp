#ifndef TP_TOKENIZER_HDR
#define TP_TOKENIZER_HDR

#include "annotator.hpp"

class TpTokenizer : public Annotator {

private:

    class Worker : LatticeWorker {
    public:
        Worker(Processor& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Processor& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice) = 0;

};

#endif

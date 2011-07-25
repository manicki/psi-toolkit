#ifndef MAIN_FACTORIES_KEEPER_HDR
#define MAIN_FACTORIES_KEEPER_HDR

#include "factories_keeper.hpp"

class MainFactoriesKeeper {

public:
    ProcessorFactory& getProcessorFactory(std::string processorName);

// singleton pattern used here
    static MainFactoriesKeeper& getInstance();

private:
    MainFactoriesKeeper();

    // undefined as it is a singleton
    MainFactoriesKeeper(const MainFactoriesKeeper &);
    MainFactoriesKeeper& operator=(const MainFactoriesKeeper&);

    FactoriesKeeper keeper_;
};

#endif

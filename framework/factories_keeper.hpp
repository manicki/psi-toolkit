#ifndef FACTORIES_KEEPER_HDR
#define FACTORIES_KEEPER_HDR

#include <map>
#include <boost/shared_ptr.hpp>

#include "processor_factory.hpp"

class FactoriesKeeper {

public:
    void takeProcessorFactory(ProcessorFactory* processorFactory);

    ProcessorFactory& getProcessorFactory(std::string processorName);

private:
    std::map<std::string, boost::shared_ptr<ProcessorFactory> > nameToFactoryMap_;

};


#endif

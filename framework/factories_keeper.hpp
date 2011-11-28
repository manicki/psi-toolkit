#ifndef FACTORIES_KEEPER_HDR
#define FACTORIES_KEEPER_HDR

#include <map>
#include <boost/shared_ptr.hpp>

#include "processor_factory.hpp"
#include "psi_exception.hpp"

class FactoriesKeeper {

public:
    void takeProcessorFactory(ProcessorFactory* processorFactory);

    ProcessorFactory& getProcessorFactory(std::string processorName);
    std::vector<std::string> getProcessorNames();

    class Exception : public PsiException  {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

    class UnknownProcessorException : public Exception {
    public:
        UnknownProcessorException(const std::string& processorName);

        virtual ~UnknownProcessorException() throw() {}
    };

private:
    std::map<std::string, boost::shared_ptr<ProcessorFactory> > nameToFactoryMap_;
};


#endif

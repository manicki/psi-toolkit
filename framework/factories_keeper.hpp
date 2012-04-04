#ifndef FACTORIES_KEEPER_HDR
#define FACTORIES_KEEPER_HDR

#include <map>
#include <boost/shared_ptr.hpp>

#include "annotator_factory.hpp"
#include "lattice_reader_factory.hpp"
#include "lattice_writer_factory.hpp"
#include "aliaser.hpp"
#include "psi_exception.hpp"

class FactoriesKeeper {

public:
    void takeProcessorFactory(ProcessorFactory* processorFactory);

    void addTagBasedAlias(const std::string& tag, const std::string& alias);
    void addTagBasedIzeAliases(const std::string& tag, const std::string& aliasRoot);

    static std::string getBaseAliasForTag(const std::string& tag);

    ProcessorFactory& getProcessorFactory(std::string processorName);
    std::vector<std::string> getProcessorNames();
    std::set<std::string> getAliasNames();

    // takes aliases into account
    std::list<ProcessorFactory*> getProcessorFactoriesForName(std::string name);

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
    void checkAnnotator_(ProcessorFactory* processorFactory);

    std::map<std::string, boost::shared_ptr<ProcessorFactory> > nameToFactoryMap_;
    Aliaser aliaser_;
};


#endif

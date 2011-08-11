#include "processor_factory.hpp"

Processor* ProcessorFactory::createProcessor(const boost::program_options::variables_map& options) {
    return doCreateProcessor(options);
}

boost::program_options::options_description ProcessorFactory::optionsHandled() {
    return doOptionsHandled();
}

std::string ProcessorFactory::getName() {
    return doGetName();
}

ProcessorFactory::~ProcessorFactory() {
}

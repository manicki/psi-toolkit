#include "processor_factory.hpp"

Processor* ProcessorFactory::createProcessor(const boost::program_options::variables_map& options) {
    return doCreateProcessor(options);
}

boost::program_options::options_description ProcessorFactory::optionsHandled() {
    return doOptionsHandled();
}

std::string ProcessorFactory::getContinuation(
    const boost::program_options::variables_map& options) const {
    return doGetContinuation(options);
}

double ProcessorFactory::getQualityScore(
    const boost::program_options::variables_map& options) const {

    return doGetQualityScore(options);
}

double ProcessorFactory::getEstimatedTime(
    const boost::program_options::variables_map& options) const {

    return doGetEstimatedTime(options);
}

double ProcessorFactory::doGetQualityScore(
    const boost::program_options::variables_map& /*options*/) const {

    return 0;
}

double ProcessorFactory::doGetEstimatedTime(
    const boost::program_options::variables_map& /*options*/) const {

    return -1.0;
}


std::string ProcessorFactory::getName() {
    return doGetName();
}

ProcessorFactory::~ProcessorFactory() {
}

#ifndef PROCESSOR_FACTORY
#define PROCESSOR_FACTORY

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "processor.hpp"

/*!
  Processor factory is used to create a given processor.
  As processors may require different options Boost Program.Options library
  is used to pass options.
*/

class ProcessorFactory {

public:
    /**
     * Creates a processor with the options specified.
     */
    Processor* createProcessor(const boost::program_options::variables_map& options);

    /**
     * Returns the description of options handled by the processor.
     */
    boost::program_options::options_description optionsHandled();

    /**
     * Name as used in the psi toolkit.
     */
    std::string getName();

private:

    virtual Processor* doCreateProcessor(const boost::program_options::variables_map& options) = 0;

    virtual boost::program_options::options_description doOptionsHandled() = 0;

    virtual std::string doGetName() = 0;

};

#endif

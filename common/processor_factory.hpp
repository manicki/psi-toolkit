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
     * Returns the pipeline fragment that will be appended if the given
     * processor is the last in the pipeline.
     *
     * In particular, should be defined for processors that are not writers.
     */
    std::string getContinuation(const boost::program_options::variables_map& options) const;

    /**
     * Returns the number measuring somehow the quality of data.
     *
     * Baseline value is 0.
     */
    double getQualityScore(const boost::program_options::variables_map& options) const;

    /**
     * Return the estimated time for processing 1000 bytes of regular text.
     *
     * Returns negative value if the estimation is unknown.
     */
    double getEstimatedTime(const boost::program_options::variables_map& options) const;

    /**
     * Name as used in the psi toolkit.
     */
    std::string getName();

    virtual ~ProcessorFactory();

private:

    virtual Processor* doCreateProcessor(
        const boost::program_options::variables_map& options) = 0;

    virtual boost::program_options::options_description doOptionsHandled() = 0;

    virtual std::string doGetContinuation(
        const boost::program_options::variables_map& options) const = 0;

    virtual double doGetQualityScore(
        const boost::program_options::variables_map& options) const;

    virtual double doGetEstimatedTime(
        const boost::program_options::variables_map& options) const;


    virtual std::string doGetName() = 0;

};

#endif

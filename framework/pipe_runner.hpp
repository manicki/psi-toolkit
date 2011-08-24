#ifndef PIPE_RUNNER_HDR
#define PIPE_RUNNER_HDR

#include "pipeline_specification.hpp"

#include "processor_factory.hpp"
#include "lattice_reader_factory.hpp"
#include "lattice_writer_factory.hpp"
#include "annotator_factory.hpp"

class PipeRunner {

public:
    PipeRunner(int argc, char* argv[]);

    int run();

private:
    PipelineSpecification pipelineSpecification_;

    void parseIntoPipelineSpecification_(int argc, char* argv[]);

    ProcessorFactory& getFactory_(const PipelineElementSpecification& elementSpec);
    LatticeReaderFactory& getReaderFactory_(const PipelineElementSpecification& elementSpec);
    LatticeWriterFactory& getWriterFactory_(const PipelineElementSpecification& elementSpec);
    AnnotatorFactory& getAnnotatorFactory_(const PipelineElementSpecification& elementSpec);

    bool isLastElement_(
        std::list<PipelineElementSpecification>::iterator it,
        PipelineSpecification& pipelineSpecification);

    boost::program_options::variables_map parseOptions_(
        const boost::program_options::options_description& optionsDescription,
        const PipelineElementSpecification& pipelineElement);

    static const std::string PIPELINE_SEPARATOR;
};

#endif

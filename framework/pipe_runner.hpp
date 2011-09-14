#ifndef PIPE_RUNNER_HDR
#define PIPE_RUNNER_HDR

#include "pipeline_specification.hpp"

#include "processor_factory.hpp"
#include "lattice_reader_factory.hpp"
#include "lattice_writer_factory.hpp"
#include "annotator_factory.hpp"

class PipeRunner {

public:
    explicit PipeRunner(const std::string& pipeline);
    PipeRunner(int argc, char* argv[]);
    PipeRunner(std::vector<std::string> args);

    int run(std::istream&, std::ostream&);

private:
    PipelineSpecification pipelineSpecification_;

    void parseIntoPipelineSpecification_(std::vector<std::string> args, bool isTheFirstArgProgramName);

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

    std::vector<std::string> splitPipeline_(const std::string& pipeline);

    static const std::string PIPELINE_SEPARATOR;
};

#endif

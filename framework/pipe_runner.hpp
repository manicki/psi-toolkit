#ifndef PIPE_RUNNER_HDR
#define PIPE_RUNNER_HDR

#include "pipeline_specification.hpp"

class PipeRunner {

public:
    PipeRunner(int argc, char* argv[]);

    int run();

private:
    PipelineSpecification pipelineSpecification_;

    void parseIntoPipelineSpecification_(int argc, char* argv[]);

    static const std::string PIPELINE_SEPARATOR;

};

#endif

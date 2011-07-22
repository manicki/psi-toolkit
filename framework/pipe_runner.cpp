#include "pipe_runner.hpp"

#include <iostream>

#include <list>

PipeRunner::PipeRunner(int argc, char* argv[]) {
    parseIntoPipelineSpecification_(argc, argv);
}

int PipeRunner::run() {
    for (std::list<PipelineElementSpecification>::iterator it = pipelineSpecification_.elements.begin();
         it != pipelineSpecification_.elements.end();
         ++it)
        std::cout << (*it).processorName << std::endl;

    return 0;
}


const std::string PipeRunner::PIPELINE_SEPARATOR = "!";

void PipeRunner::parseIntoPipelineSpecification_(int argc, char* argv[]) {

    bool nameExpected = true;

    for (int i = 1; i < argc; ++i) {

        std::string arg(argv[i]);

        if (i == 1 || arg == PIPELINE_SEPARATOR) {
            nameExpected = true;
            pipelineSpecification_.elements.push_back(PipelineElementSpecification());
        }

        if (arg == PIPELINE_SEPARATOR) {
            ;
        }
        else if (nameExpected) {
            pipelineSpecification_.elements.back().processorName = arg;
            nameExpected = false;
        }
        else {
            pipelineSpecification_.elements.back().processorArgs.push_back(arg);
        }
    }
}

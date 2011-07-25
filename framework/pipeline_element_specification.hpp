#ifndef PIPELINE_ELEMENT_SPECIFICATION_HDR
#define PIPELINE_ELEMENT_SPECIFICATION_HDR

#include <string>
#include <list>

struct PipelineElementSpecification {

    std::string processorName;
    std::list<std::string> processorArgs;

};

#endif

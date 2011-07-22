#ifndef PIPELINE_SPECIFICATION_HDR
#define PIPELINE_SPECIFICATION_HDR

#include <list>

#include "pipeline_element_specification.hpp"

struct PipelineSpecification {

    std::list<PipelineElementSpecification> elements;

};

#endif

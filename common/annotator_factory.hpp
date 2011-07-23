#ifndef ANNOTATOR_FACTORY_HDR
#define ANNOTATOR_FACTORY_HDR

#include "processor_factory.hpp"
#include "annotator.hpp"

class AnnotatorFactory : public ProcessorFactory {
    
public:
    Annotator* createAnnotator(boost::program_options::variables_map options);

private:
    virtual Annotator* doCreateProcessor(boost::program_options::variables_map options);

    virtual Annotator* doCreateAnnotator(boost::program_options::variables_map options) = 0;
};

#endif

#include "pipe_runner.hpp"

#include <iostream>
#include <list>
#include <boost/scoped_ptr.hpp>

#include "main_factories_keeper.hpp"

PipeRunner::PipeRunner(int argc, char* argv[]) {
    parseIntoPipelineSpecification_(argc, argv);
}

int PipeRunner::run() {
    Lattice lattice("");

    std::list<PipelineElementSpecification>::iterator it = pipelineSpecification_.elements.begin();

    LatticeReaderFactory& readerFactory = getReaderFactory_(*it);
    boost::scoped_ptr<LatticeReader> reader(readerFactory.createLatticeReader(
                                                boost::program_options::variables_map()));
    reader->readIntoLattice(std::cin, lattice);
    ++it;

    for (;
         it != pipelineSpecification_.elements.end();
         ++it) {

        if (isLastElement_(it, pipelineSpecification_)) {
            LatticeWriterFactory& writerFactory = getWriterFactory_(*it);
            boost::scoped_ptr<LatticeWriter> writer(writerFactory.createLatticeWriter(
                                                        boost::program_options::variables_map()));
            writer->writeLattice(lattice, std::cout);
        }
        else {
            AnnotatorFactory& annotatorFactory = getAnnotatorFactory_(*it);
            boost::scoped_ptr<Annotator> annotator(annotatorFactory.createAnnotator(
                                                       boost::program_options::variables_map()));
            annotator->annotate(lattice);
        }
    }

    return 0;
}

const std::string PipeRunner::PIPELINE_SEPARATOR = "!";

ProcessorFactory& PipeRunner::getFactory_(const PipelineElementSpecification& elementSpec) {
    return MainFactoriesKeeper::getInstance().getProcessorFactory(elementSpec.processorName);
}

LatticeReaderFactory& PipeRunner::getReaderFactory_(const PipelineElementSpecification& elementSpec) {
    return dynamic_cast<LatticeReaderFactory&>(getFactory_(elementSpec));
}

LatticeWriterFactory& PipeRunner::getWriterFactory_(const PipelineElementSpecification& elementSpec) {
    return dynamic_cast<LatticeWriterFactory&>(getFactory_(elementSpec));
}

AnnotatorFactory& PipeRunner::getAnnotatorFactory_(const PipelineElementSpecification& elementSpec) {
    return dynamic_cast<AnnotatorFactory&>(getFactory_(elementSpec));
}


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

bool PipeRunner::isLastElement_(
    std::list<PipelineElementSpecification>::iterator it,
    PipelineSpecification& pipelineSpecification) {
    ++it;
    return it == pipelineSpecification.elements.end();
}



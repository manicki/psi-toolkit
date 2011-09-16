#include "pipe_runner.hpp"

#include <iostream>
#include <list>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/algorithm/string.hpp>

#include "main_factories_keeper.hpp"

#include "logging.hpp"

PipeRunner::PipeRunner(const std::string& pipeline) {
    parseIntoPipelineSpecification_(splitPipeline_(pipeline), false);
}

PipeRunner::PipeRunner(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    parseIntoPipelineSpecification_(args, true);
}

PipeRunner::PipeRunner(std::vector<std::string> args) {
    parseIntoPipelineSpecification_(args, false);
}

int PipeRunner::run(std::istream& in, std::ostream& out) {
    Lattice lattice;

    std::list<PipelineElementSpecification>::iterator it = pipelineSpecification_.elements.begin();

    boost::program_options::variables_map options;

    LatticeReaderFactory& readerFactory = getReaderFactory_(*it);
    boost::scoped_ptr<LatticeReader> reader(readerFactory.createLatticeReader(options));

    reader->readIntoLattice(in, lattice);
    ++it;

    for (;
         it != pipelineSpecification_.elements.end();
         ++it) {

        if (isLastElement_(it, pipelineSpecification_)) {
            LatticeWriterFactory& writerFactory = getWriterFactory_(*it);
            boost::program_options::variables_map options
                = parseOptions_(writerFactory.optionsHandled(), *it);

            boost::scoped_ptr<LatticeWriter> writer(writerFactory.createLatticeWriter(options));

            writer->writeLattice(lattice, out);
        }
        else {
            AnnotatorFactory& annotatorFactory = getAnnotatorFactory_(*it);
            boost::program_options::variables_map options
                = parseOptions_(annotatorFactory.optionsHandled(), *it);

            boost::scoped_ptr<Annotator> annotator(annotatorFactory.createAnnotator(options));

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


void PipeRunner::parseIntoPipelineSpecification_(
    std::vector<std::string> args, bool isTheFirstArgProgramName) {

    bool nameExpected = true;

    size_t startingIndex = (isTheFirstArgProgramName ? 1 : 0);

    for (size_t i = startingIndex; i < args.size(); ++i) {

        if (i == startingIndex || args[i] == PIPELINE_SEPARATOR) {
            nameExpected = true;
            pipelineSpecification_.elements.push_back(PipelineElementSpecification());
        }

        if (args[i] == PIPELINE_SEPARATOR) {
            ;
        }
        else if (nameExpected) {
            pipelineSpecification_.elements.back().processorName = args[i];
            nameExpected = false;
        }
        else {
            pipelineSpecification_.elements.back().processorArgs.push_back(args[i]);
        }
    }
}

bool PipeRunner::isLastElement_(
    std::list<PipelineElementSpecification>::iterator it,
    PipelineSpecification& pipelineSpecification) {
    ++it;
    return it == pipelineSpecification.elements.end();
}

boost::program_options::variables_map PipeRunner::parseOptions_(
    const boost::program_options::options_description& optionsDescription,
    const PipelineElementSpecification& pipelineElement) {

    const std::list<std::string>& processorArgs = pipelineElement.processorArgs;

    int argc = processorArgs.size() + 1;
    boost::scoped_array<char*> argv(new char* [argc + 1]);

    size_t i = 1;
    for(std::list<std::string>::const_iterator iter = processorArgs.begin();
        iter != processorArgs.end();
        ++iter,++i)
        argv[i] = const_cast<char*>((*iter).c_str()); //trick necessary because of Boost 1.42 bug
    argv[argc] = 0;

    boost::program_options::variables_map options;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv.get(),
                                                   optionsDescription), options);
    boost::program_options::notify(options);

    return options;
}

std::vector<std::string> PipeRunner::splitPipeline_(const std::string& pipeline) {
    std::vector<std::string> strs;
    // the first element should be the name of the program
    boost::split(strs, pipeline, boost::is_any_of(" "));
    return strs;
}

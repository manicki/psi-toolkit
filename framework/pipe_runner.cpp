#include "pipe_runner.hpp"

#include <iostream>
#include <list>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "main_factories_keeper.hpp"

#include "logging.hpp"

PipeRunner::PipeRunner(const std::string& pipeline) {
    parseIntoGraph_(splitPipeline_(pipeline), false);
}

PipeRunner::PipeRunner(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    parseIntoGraph_(args, true);
}

PipeRunner::PipeRunner(std::vector<std::string> args) {
    parseIntoGraph_(args, false);
}

int PipeRunner::run(std::istream& in, std::ostream& out) {
    Lattice lattice;

    PipelineGraph::vertex_descriptor current = firstNode;

    do {
        runPipelineNode_(current, lattice, in, out);
        if (!goToNextNode_(current))
            break;
    } while (1);

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

void PipeRunner::parseIntoGraph_(std::vector<std::string> args, bool isTheFirstArgProgramName) {
    parseIntoPipelineSpecification_(args, isTheFirstArgProgramName);
    pipelineSpecification2Graph_();
    completeGraph_();
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

void PipeRunner::pipelineSpecification2Graph_() {
    bool isFirst = true;
    PipelineGraph::vertex_descriptor currentVertex;

    BOOST_FOREACH(const PipelineElementSpecification& element, pipelineSpecification_.elements) {
        PipelineGraph::vertex_descriptor newVertex =
            boost::add_vertex(
                pipelineElement2Node_(element),
                pipelineGraph_);

        if (isFirst) {
            isFirst = false;
            firstNode = newVertex;
        }
        else {
            std::string emptyString;
            boost::add_edge(currentVertex, newVertex, emptyString, pipelineGraph_);
        }

        currentVertex = newVertex;
    }

    lastNode = currentVertex;
}

void PipeRunner::completeGraph_() {
}

void PipeRunner::runPipelineNode_(
    PipelineGraph::vertex_descriptor current,
    Lattice& lattice, std::istream& in, std::ostream& out) {

    PipelineNode& currentPipelineNode = pipelineGraph_[current];
    currentPipelineNode.createProcessor();

    if (current == firstNode) {
        boost::shared_ptr<LatticeReader> reader =
            boost::dynamic_pointer_cast<LatticeReader>(
                currentPipelineNode.getProcessor());
        reader->readIntoLattice(in, lattice);
    }
    else if (current == lastNode) {
        boost::shared_ptr<LatticeWriter> writer =
            boost::dynamic_pointer_cast<LatticeWriter>(
                currentPipelineNode.getProcessor());
        writer->writeLattice(lattice, out);
    }
    else {
        boost::shared_ptr<Annotator> annotator =
            boost::dynamic_pointer_cast<Annotator>(
                currentPipelineNode.getProcessor());
        annotator->annotate(lattice);
    }
}

bool PipeRunner::goToNextNode_(PipelineGraph::vertex_descriptor& current) {
    std::pair<boost::graph_traits<PipelineGraph>::out_edge_iterator,
              boost::graph_traits<PipelineGraph>::out_edge_iterator> iterPair
        = boost::out_edges(current, pipelineGraph_);

    if (iterPair.first == iterPair.second)
        return false;
    else {
        current = boost::target(*iterPair.first, pipelineGraph_);

        ++iterPair.first;
        if (iterPair.first != iterPair.second)
            WARN("unexpected fork in pipeline graph");
    }

    return true;
}

PipeRunner::PipelineNode PipeRunner::pipelineElement2Node_(const PipelineElementSpecification& element) {
    ProcessorFactory& factory = getFactory_(element);

    return PipelineNode(
        factory,
        parseOptions_(factory.optionsHandled(), element));
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

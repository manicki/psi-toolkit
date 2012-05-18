#include "bracketing_lattice_writer.hpp"


#include <algorithm>
#include <iterator>

#include "active_elements_printer.hpp"


std::string BracketingLatticeWriter::getFormatName() {
    return "bracketing";
}


LatticeWriter<std::ostream>* BracketingLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    std::vector<std::string> showOnlyTags;
    std::vector<std::string> filter;
    std::vector<std::string> showAttributes;

    if (options.count("show-only-tags")) {
        showOnlyTags = options["show-only-tags"].as< std::vector<std::string> >();
    } else if (options.count("filter")) {
        showOnlyTags = options["filter"].as< std::vector<std::string> >();
    }

    if (options.count("filter")) {
        filter = options["filter"].as< std::vector<std::string> >();
    }
    if (options.count("show-attributes")) {
        showAttributes = options["show-attributes"].as< std::vector<std::string> >();
    }

    return new BracketingLatticeWriter(
        options["opening-bracket"].as<std::string>(),
        options["closing-bracket"].as<std::string>(),
        options["tag-separator"].as<std::string>(),
        showOnlyTags,
        filter,
        options["av-pairs-separator"].as<std::string>(),
        options["av-separator"].as<std::string>(),
        showAttributes
    );
}


boost::program_options::options_description BracketingLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("opening-bracket",
            boost::program_options::value<std::string>()->default_value("["),
            "the actual format of opening brackets")
        ("closing-bracket",
            boost::program_options::value<std::string>()->default_value("]"),
            "the actual format of closing brackets")
        ("tag-separator",
            boost::program_options::value<std::string>()->default_value(","),
            "separates tags")
        ("show-only-tags",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "limits the tag names that will appear in `%T` substitions")
        ("filter",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "filters the edges by tags")
        ("av-pairs-separator",
            boost::program_options::value<std::string>()->default_value(","),
            "separates the attribute-value pairs")
        ("av-separator",
            boost::program_options::value<std::string>()->default_value("="),
            "separates the attribute and its value")
        ("show-attributes",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "the attributes to be shown")
        ;

    return optionsDescription;
}


std::string BracketingLatticeWriter::Factory::doGetName() const {
    return "bracketing-writer";
}


boost::filesystem::path BracketingLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}


std::set<std::string> BracketingLatticeWriter::intersectOnlyTags(
    std::set<std::string> tags
) {
    if (showOnlyTags_.empty()) {
        return tags;
    }
    std::set<std::string> result;
    set_intersection(
        showOnlyTags_.begin(), showOnlyTags_.end(),
        tags.begin(), tags.end(),
        std::inserter(result, result.begin())
    );
    return result;
}


std::map<std::string, std::string> BracketingLatticeWriter::filterAttributes(
    std::map<std::string, std::string> avMap
) {
    if (showAttributes_.empty()) {
        return avMap;
    }
    std::map<std::string, std::string> result;
    typedef std::pair<std::string, std::string> StrStrPair;
    BOOST_FOREACH(StrStrPair avPair, avMap) {
        if (showAttributes_.count(avPair.first)) {
            result.insert(avPair);
        }
    }
    return result;
}


std::string BracketingLatticeWriter::doInfo() {
    return "Bracketing writer";
}


BracketingLatticeWriter::Worker::Worker(BracketingLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}


void BracketingLatticeWriter::Worker::doRun() {

    ActiveElementsPrinter::Manager aepManager(
        processor_.getTagSeparator(),
        processor_.getAVPairsSeparator(),
        processor_.getAVSeparator()
    );

    ActiveElementsPrinter aepOpen = aepManager.getPrinter(processor_.getOpeningBracket());
    ActiveElementsPrinter aepClose = aepManager.getPrinter(processor_.getClosingBracket());

    LayerTagMask mask = lattice_.getLayerTagManager().getMask(processor_.getFilter());

    std::stack<Lattice::EdgeDescriptor> edgeStack;
    std::set<Lattice::EdgeDescriptor> edgeSet;

    Lattice::VertexIterator vi(lattice_);

    while (vi.hasNext()) {

        Lattice::VertexDescriptor vertex = vi.next();

        Lattice::InOutEdgesIterator iei = lattice_.inEdges(vertex, mask);
        while (iei.hasNext()) {
            edgeSet.insert(iei.next());
        }
        while (!edgeStack.empty()) {
            Lattice::EdgeDescriptor edge = edgeStack.top();
            if (edgeSet.count(edge)) {
                edgeStack.pop();
                std::list<std::string> tagsList
                    = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
                std::set<std::string> tags(tagsList.begin(), tagsList.end());
                AnnotationItem annotationItem = lattice_.getEdgeAnnotationItem(edge);
                alignOutput_(aepClose.print(
                    processor_.intersectOnlyTags(tags),
                    annotationItem.getCategory(),
                    annotationItem.getText(),
                    processor_.filterAttributes(
                        lattice_.getAnnotationItemManager().getAVMap(annotationItem)
                    ),
                    lattice_.getEdgeScore(edge)
                ));
            } else {
                break;
            }
        }

        Lattice::InOutEdgesIterator oei = lattice_.outEdges(vertex, mask);
        PriorityStack priorityStack;
        while (oei.hasNext()) {
            Lattice::EdgeDescriptor edge = oei.next();
            priorityStack.push(edge, lattice_.getEdgeLength(edge));
        }
        while (!priorityStack.empty()) {
            Lattice::EdgeDescriptor edge = priorityStack.pop();
            std::list<std::string> tagsList
                = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
            std::set<std::string> tags(tagsList.begin(), tagsList.end());
            AnnotationItem annotationItem = lattice_.getEdgeAnnotationItem(edge);
            alignOutput_(aepOpen.print(
                processor_.intersectOnlyTags(tags),
                annotationItem.getCategory(),
                annotationItem.getText(),
                processor_.filterAttributes(
                    lattice_.getAnnotationItemManager().getAVMap(annotationItem)
                ),
                lattice_.getEdgeScore(edge)
            ));
            edgeStack.push(edge);
        }

        try {
            alignOutput_(lattice_.getEdgeText(
                lattice_.firstOutEdge(vertex, lattice_.getLayerTagManager().getMask("symbol"))
            ));
        } catch (NoEdgeException) { }

    }

    alignOutputNewline_();

}


BracketingLatticeWriter::Worker::~Worker() {
}


void BracketingLatticeWriter::PriorityStack::push(
    Lattice::EdgeDescriptor element,
    size_t priority
) {
    if (priority > stacks_.size()) {
        stacks_.resize(priority + 1);
    }
    if (priority > maxPriority_) {
        maxPriority_ = priority;
    }
    stacks_[priority].push(element);
}


Lattice::EdgeDescriptor BracketingLatticeWriter::PriorityStack::pop() {
    Lattice::EdgeDescriptor result = stacks_[maxPriority_].top();
    stacks_[maxPriority_].pop();
    while (maxPriority_ > 0 && stacks_[maxPriority_].empty()) {
        maxPriority_--;
    }
    return result;
}


bool BracketingLatticeWriter::PriorityStack::empty() {
    return maxPriority_ == 0;
}

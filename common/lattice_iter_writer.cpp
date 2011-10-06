#include "lattice_iter_writer.hpp"

void LatticeIterWriter::run() {

    Lattice::VertexIterator vi(lattice_);
    Lattice::VertexDescriptor vd = lattice_.getFirstVertex();
    Lattice::EdgeDescriptor edge;
    std::queue<Lattice::EdgeDescriptor> basicTagEdges;

    std::map<std::string, Lattice::VertexDescriptor> targets;

    while (linear_ ? true : vi.hasNext()) {
        if (!linear_) {
            vd = vi.next();
        }
        Lattice::InOutEdgesIterator oei
            = lattice_.outEdges(vd, lattice_.getLayerTagManager().anyTag());
        while(oei.hasNext()) {
            edge = oei.next();
            std::list<std::string> tags
                = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
            BOOST_FOREACH(std::string tag, tags) {
                if (
                    basicTag_ == tag &&
                    (withBlank_ || !boost::algorithm::trim_copy(lattice_.getEdgeText(edge)).empty())
                ) {
                    basicTagEdges.push(edge);
                }
                if (isHandledTag_(tag) && targets[tag] == vd) {
                    targets[tag] = lattice_.getEdgeTarget(edge);
                    outputIterator_.closeGroup(tag);
                    outputIterator_.openGroup(tag);
                }
            }
        }
        outputIterator_.openAlternative();
        while (!basicTagEdges.empty()) {
            Lattice::EdgeDescriptor basicTagEdge = basicTagEdges.front();
            if (lattice_.getAnnotationText(basicTagEdge).empty()) {
                outputIterator_.putElement(
                    lattice_.getAnnotationCategory(basicTagEdge)
                );
            } else {
                outputIterator_.putElement(
                    lattice_.getAnnotationText(basicTagEdge)
                );
            }
            basicTagEdges.pop();
            if (noAlts_) {
                while (!basicTagEdges.empty()) {
                    basicTagEdges.pop();
                }
            }
        }
        outputIterator_.closeAlternative();
        if (linear_) {
            try {
                vd = lattice_.getEdgeTarget(
                    lattice_.firstOutEdge(vd, lattice_.getLayerTagManager().anyTag())
                );
            } catch (NoEdgeException) {
                break;
            }
        }
    }

    typedef std::pair<std::string, Lattice::VertexDescriptor> TagTargetPair;
    BOOST_FOREACH(TagTargetPair tagTargetpair, targets) {
        outputIterator_.closeGroup(tagTargetpair.first);
    }

    outputIterator_.flush();

}

bool LatticeIterWriter::isHandledTag_(std::string tagName) {
    return std::find(handledTags_.begin(), handledTags_.end(), tagName) != handledTags_.end();
}

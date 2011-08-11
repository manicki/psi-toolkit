
#include "lattice_wrapper.hpp"

namespace poleng {

    namespace bonsai {
        namespace puddle {

            LatticeWrapper::LatticeWrapper(TagsetPtr tagset) {
                initAnnotationItemManager(tagset);
            }

            ParseGraphPtr LatticeWrapper::readInputLattice(Lattice &lattice,
                    std::string &sentenceString) {
                ParseGraphPtr pg = ParseGraphPtr(new ParseGraph());

                std::map<int, TransitionInfo*> edgesMap;

                LayerTagCollection token_tag
                    = lattice.getLayerTagManager().createSingletonTagCollection("token");
                LayerTagCollection lemma_tag
                    = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
                LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);

                Lattice::EdgesSortedBySourceIterator edgeIterator =
                    lattice.edgesSortedBySource(lemmaMask);
                while (edgeIterator.hasNext()) {
                    Lattice::EdgeDescriptor edge = edgeIterator.next();
                    int start = lattice.getEdgeBeginIndex(edge);
                    int end = start + lattice.getEdgeLength(edge);
                    std::string orth = lattice.getEdgeText(edge);
                    AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
                    std::string partOfSpeech = ai.getCategory();
                    std::string base;
                    std::string morphology = "";
                    std::list< std::pair<std::string, std::string> > av
                        = annotationItemManager.getValues(ai);
                    for (std::list< std::pair<std::string, std::string> >
                            ::iterator avi = av.begin();
                            avi != av.end(); avi ++) {
                        if (avi->first == "base")
                            base = avi->second;
                        else {
                            if (morphology != "")
                                morphology += ":";
                            morphology += avi->second;
                        }
                    }

                    std::string morphoString = partOfSpeech;
                    if (morphology != "")
                        morphoString += ":" + morphology;
                    PosInfo pi(base, morphoString, 1);

                    std::map<int, TransitionInfo*>::iterator edgesMapIt =
                        edgesMap.find(start);
                    if (edgesMapIt != edgesMap.end()) {
                        edgesMapIt->second->addMorphology(pi);
                    } else {
                        TransitionInfo *edge = new TransitionInfo("token");
                        edge->setStart(start);
                        edge->setEnd(end);
                        edge->setDepth(0);
                        edge->setLabel(orth);
                        edge->setOrth(orth);
                        edge->addMorphology(pi);
                        edgesMap.insert(std::pair<int, TransitionInfo*>(
                                    start, edge));
                    }
                }

                sentenceString = "<<s<0<0<sb<>";
                for (std::map<int, TransitionInfo*>::iterator edgesMapIt =
                        edgesMap.begin();
                        edgesMapIt != edgesMap.end(); edgesMapIt ++) {
                    pg->add_edge(edgesMapIt->second->getStart(),
                            edgesMapIt->second->getEnd(),
                            *(edgesMapIt->second));
                    std::stringstream ss;
                    ss << "<<t" << "<" << edgesMapIt->second->getStart()
                        << "<" << edgesMapIt->second->getEnd()
                        << "<" << "TOKEN" << "<" << edgesMapIt->second->getOrth();
                    std::vector<PosInfo> vars = edgesMapIt->second->variants_;
                    for (std::vector<PosInfo>::iterator varIt = vars.begin();
                            varIt != vars.end(); varIt ++ )
                        ss << "<" << boost::get<0>(*varIt) << "<"
                            << boost::get<1>(*varIt);
                    ss << ">";
                    sentenceString += ss.str();
                }
                sentenceString += "<<s<666<666<se<>";

//                std::cerr << "TAKI LUJ: "<< pg->write_graphviz() << std::endl;
                return pg;
            }


            void LatticeWrapper::initAnnotationItemManager(TagsetPtr tagset) {
                AnnotationItem ai("base");
                annotationItemManager.setValue(ai, "base", "xxx");
                std::vector<std::string> attributes = tagset->getAttributes();
                std::vector<std::string> partsOfSpeech =
                    tagset->getPartsOfSpeech();
                for (std::vector<std::string>::iterator it = attributes.begin();
                        it != attributes.end(); it ++) {
                    annotationItemManager.setValue(ai, *it, "aaa");
                }
            }

           /* std::list<Lattice::EdgeDescriptor> LatticeWrapper::getEdge(
                    Lattice &lattice, Lattice::VertexDescriptor start,
                    LayerTagMask mask) {

            }

            void LatticeWrapper::addParseEdge(Lattice &lattice,
                    Lattice::VertexDescriptor start, Lattice::VertexDescriptor end,
                    std::string &parseLabel) {

            }*/

        }
    }
}

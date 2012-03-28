#include "nkjp_lattice_reader.hpp"

#include <cstring>
#include <locale>

#include <boost/algorithm/string/trim.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "string_frag.hpp"

#include "logging.hpp"


std::string NKJPLatticeReader::getFormatName() {
    return "NKJP";
}

std::string NKJPLatticeReader::doInfo() {
    return "NKJP reader";
}

NKJPLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* NKJPLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /* options */
) {
    return new NKJPLatticeReader();
}

boost::program_options::options_description NKJPLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string NKJPLatticeReader::Factory::doGetName() {
    return "nkjp-reader";
}

boost::filesystem::path NKJPLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

NKJPLatticeReader::Worker::Worker(
    NKJPLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor)
{ }

void NKJPLatticeReader::Worker::doRun() {

    // SET_LOGGING_LEVEL("DEBUG");
    boost::property_tree::ptree xpt;
    boost::property_tree::read_xml(inputStream_, xpt);

    int prevEnding = 0;
    BOOST_FOREACH(
        boost::property_tree::ptree::value_type &vP,
        xpt.get_child("teiCorpus.TEI.text.body")
    ) if (
        strcmp(vP.first.data(), "p") == 0 ||
        strcmp(vP.first.data(), "ab") == 0 ||
        strcmp(vP.first.data(), "u") == 0
    ) {
        Lattice::VertexDescriptor pBegin = lattice_.getLastVertex();
        Lattice::VertexDescriptor pEnd = lattice_.getLastVertex();
        Lattice::EdgeSequence::Builder pBuilder(lattice_);
        BOOST_FOREACH(
            boost::property_tree::ptree::value_type &vS,
            vP.second.get_child("")
        ) if (strcmp(vS.first.data(), "s") == 0) {
            Lattice::VertexDescriptor sBegin = lattice_.getLastVertex();
            Lattice::VertexDescriptor sEnd = lattice_.getLastVertex();
            Lattice::EdgeSequence::Builder sBuilder(lattice_);
            std::string segText;
            BOOST_FOREACH(
                boost::property_tree::ptree::value_type &vSeg,
                vS.second.get_child("")
            ) if (strcmp(vSeg.first.data(), "seg") == 0) {

                // hinting space insertion
                bool insertSpace = false;
                std::string corresp(vSeg.second.get("<xmlattr>.corresp", ""));
                std::string::const_iterator correspBegin = corresp.begin();
                std::string::const_iterator correspEnd = corresp.end();
                NKJPSegmentationCorrespGrammar correspGrammar;
                std::string comment(vSeg.second.get("fs.<xmlcomment>", ""));
                std::string::const_iterator commentBegin = comment.begin();
                std::string::const_iterator commentEnd = comment.end();
                NKJPMorphosyntaxCommentGrammar commentGrammar;
                NKJPSpaceHintItem item;
                if (
                    parse(correspBegin, correspEnd, correspGrammar, item) ||
                    parse(commentBegin, commentEnd, commentGrammar, item)
                ) {
                    if (item.beginning - prevEnding > 0) {
                        insertSpace = true;
                    } else {
                        insertSpace = false;
                    }
                    prevEnding = item.beginning + item.length;
                } else {
                    insertSpace = false;
                }

                std::string mainTag("token");
                std::string segment(vSeg.second.get("fs.f.string", segText));
                std::string npsValue(vSeg.second.get("<xmlattr>.nkjp:nps", ""));
                std::string disambId;
                BOOST_FOREACH(
                    boost::property_tree::ptree::value_type &vFS,
                    vSeg.second.get_child("")
                ) if (strcmp(vFS.first.data(), "fs") == 0) {
                    mainTag = vFS.second.get("<xmlattr>.type", mainTag);
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vF,
                        vFS.second.get_child("")
                    ) if (strcmp(vF.first.data(), "f") == 0) {
                        if (vF.second.get("<xmlattr>.name", std::string())=="orth") {
                            segment = vF.second.get("string", segment);
                        } else if (vF.second.get("<xmlattr>.name", std::string())=="disamb") {
                            BOOST_FOREACH(
                                boost::property_tree::ptree::value_type &vFF,
                                vF.second.get_child("fs")
                            ) if (strcmp(vFF.first.data(), "f") == 0) {
                                if (vFF.second.get("<xmlattr>.name", std::string())=="choice") {
                                    disambId
                                        = vFF.second.get("<xmlattr>.fVal", std::string()).substr(1);
                                    // substr(1) cuts off the first "#" character
                                }
                            }
                        }
                    }
                }
                AnnotationItem segItem(
                    vSeg.second.get("<xmlattr>.xml:id", "seg"),
                    StringFrag(segment)
                );
                BOOST_FOREACH(
                    boost::property_tree::ptree::value_type &vFS,
                    vSeg.second.get_child("")
                ) if (strcmp(vFS.first.data(), "fs") == 0) {
                    mainTag = vFS.second.get("<xmlattr>.type", mainTag);
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vF,
                        vFS.second.get_child("")
                    ) if (strcmp(vF.first.data(), "f") == 0) {
                        std::string attr(vF.second.get("<xmlattr>.name", std::string()));
                        std::string value(
                            vF.second.get("string",
                                vF.second.get("symbol.<xmlattr>.value",
                                    vF.second.get("binary.<xmlattr>.value", std::string()))));
                        if (!attr.empty() && !value.empty()) {
                            if (attr == "nps") {
                                npsValue = value;
                            } else if (attr != "orth") {
                                lattice_.getAnnotationItemManager().setValue(segItem, attr, value);
                            }
                        }
                    }
                }
                if (!npsValue.empty()) {
                    lattice_.getAnnotationItemManager().setValue(segItem, "nps", npsValue);
                }
                Lattice::EdgeDescriptor segEdge
                    = appendSegmentToLattice_(segment, segItem, mainTag, insertSpace);
                Lattice::VertexDescriptor segBegin = lattice_.getEdgeSource(segEdge);
                Lattice::VertexDescriptor segEnd = lattice_.getEdgeTarget(segEdge);

                BOOST_FOREACH(
                    boost::property_tree::ptree::value_type &vFS,
                    vSeg.second.get_child("")
                ) if (strcmp(vFS.first.data(), "fs") == 0) {
                    mainTag = vFS.second.get("<xmlattr>.type", mainTag);
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vF,
                        vFS.second.get_child("")
                    ) if (strcmp(vF.first.data(), "f") == 0) {
                        if (vF.second.get("<xmlattr>.name", std::string())=="interps") {
                            BOOST_FOREACH(
                                boost::property_tree::ptree::value_type &vFFS,
                                vF.second.get_child("")
                            ) if (strcmp(vFFS.first.data(), "fs") == 0) {
                                std::string base(vF.second.get("f.string", std::string()));
                                BOOST_FOREACH(
                                    boost::property_tree::ptree::value_type &vFF,
                                    vFFS.second.get_child("")
                                ) if (strcmp(vFF.first.data(), "f") == 0) {
                                    if (vFF.second.get("<xmlattr>.name", std::string())=="base") {
                                        base = vFF.second.get("string", "base");
                                    }
                                }
                                Lattice::EdgeSequence::Builder baseBuilder(lattice_);
                                baseBuilder.addEdge(segEdge);
                                AnnotationItem baseItem(base, StringFrag(base));
                                Lattice::EdgeDescriptor baseEdge = lattice_.addEdge(
                                    segBegin,
                                    segEnd,
                                    baseItem,
                                    getTags_("base"),
                                    baseBuilder.build()
                                );
                                BOOST_FOREACH(
                                    boost::property_tree::ptree::value_type &vFF,
                                    vFFS.second.get_child("")
                                ) if (strcmp(vFF.first.data(), "f") == 0) {
                                    if (vFF.second.get("<xmlattr>.name", std::string())=="ctag") {
                                        Lattice::EdgeSequence::Builder ctagBuilder(lattice_);
                                        ctagBuilder.addEdge(baseEdge);
                                        AnnotationItem ctagItem(
                                            vFF.second.get("symbol.<xmlattr>.value", "ctag"),
                                            StringFrag(base + "+" +
                                                vFF.second.get("symbol.<xmlattr>.value", "ctag")
                                            )
                                        );
                                        lattice_.addEdge(
                                            segBegin,
                                            segEnd,
                                            ctagItem,
                                            getTags_("ctag"),
                                            ctagBuilder.build()
                                        );
                                    } else if (
                                        vFF.second.get("<xmlattr>.name", std::string())=="msd"
                                    ) {
                                        try {
                                            Lattice::EdgeSequence::Builder msdBuilder(lattice_);
                                            msdBuilder.addEdge(baseEdge);
                                            AnnotationItem msdItem(
                                                vFF.second.get<std::string>(
                                                    "symbol.<xmlattr>.xml:id"
                                                ),
                                                StringFrag(
                                                    lattice_.getAllText(),
                                                    segBegin,
                                                    segEnd-segBegin
                                                )
                                            );
                                            lattice_.getAnnotationItemManager().setValue(
                                                msdItem,
                                                "value",
                                                vFF.second.get<std::string>(
                                                    "symbol.<xmlattr>.value"
                                                )
                                            );
                                            lattice_.addEdge(
                                                segBegin,
                                                segEnd,
                                                msdItem,
                                                getTags_("msd"),
                                                msdBuilder.build()
                                            );
                                        } catch (boost::property_tree::ptree_bad_path) {
                                            BOOST_FOREACH(
                                                boost::property_tree::ptree::value_type &vVAlt,
                                                vFF.second.get_child("vAlt")
                                            ) if (strcmp(vVAlt.first.data(), "symbol") == 0) {
                                                Lattice::EdgeSequence::Builder
                                                    vAltBuilder(lattice_);
                                                vAltBuilder.addEdge(baseEdge);
                                                AnnotationItem vAltItem(
                                                    vVAlt.second.get("<xmlattr>.xml:id", "msd"),
                                                    StringFrag(
                                                        lattice_.getAllText(),
                                                        segBegin,
                                                        segEnd-segBegin
                                                    )
                                                );
                                                lattice_.getAnnotationItemManager().setValue(
                                                    vAltItem,
                                                    "value",
                                                    vVAlt.second.get("<xmlattr>.value", "msd")
                                                );
                                                Lattice::EdgeDescriptor vAltEdge = lattice_.addEdge(
                                                    segBegin,
                                                    segEnd,
                                                    vAltItem,
                                                    getTags_("msd"),
                                                    vAltBuilder.build()
                                                );
                                                if (
                                                    !disambId.empty() &&
                                                    disambId != vVAlt.second.get(
                                                        "<xmlattr>.xml:id",
                                                        std::string()
                                                    )
                                                ) {
                                                    lattice_.discard(vAltEdge);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                sBuilder.addEdge(segEdge);
                sEnd = segEnd;

            } else if (strcmp(vSeg.first.data(), "<xmlcomment>") == 0) {
                segText = boost::algorithm::trim_copy(vSeg.second.get<std::string>(""));
            }
            AnnotationItem sItem(
                "s",
                StringFrag(lattice_.getAllText(), sBegin, sEnd-sBegin)
            );
            Lattice::EdgeDescriptor sEdge = lattice_.addEdge(
                sBegin,
                sEnd,
                sItem,
                getTags_("sentence"),
                sBuilder.build()
            );
            pBuilder.addEdge(sEdge);
            pEnd = sEnd;
        }
        AnnotationItem pItem(
            vP.first.data(),
            StringFrag(lattice_.getAllText(), pBegin, pEnd-pBegin)
        );
        lattice_.addEdge(
            pBegin,
            pEnd,
            pItem,
            getTags_("paragraph"),
            pBuilder.build()
        );
    } else if (
        strcmp(vP.first.data(), "<xmlattr>") != 0 &&
        strcmp(vP.first.data(), "<xmlcomment>") != 0
    ) {
        Lattice::VertexDescriptor divBegin = lattice_.getLastVertex();
        Lattice::EdgeSequence::Builder divBuilder(lattice_);
        BOOST_FOREACH(
            boost::property_tree::ptree::value_type &vAB,
            vP.second.get_child("")
        ) if (
            strcmp(vAB.first.data(), "<xmlattr>") != 0 &&
            strcmp(vAB.first.data(), "<xmlcomment>") != 0
        ) {
            std::string abText(vAB.second.get<std::string>(""));
            AnnotationItem abItem(vAB.first.data(), abText);
            Lattice::EdgeDescriptor abEdge = appendSegmentToLattice_(abText, abItem, "paragraph");
            divBuilder.addEdge(abEdge);
        }
        Lattice::VertexDescriptor divEnd = lattice_.getLastVertex();
        AnnotationItem divItem(
            vP.first.data(),
            StringFrag(lattice_.getAllText(), divBegin, divEnd-divBegin)
        );
        lattice_.addEdge(
            divBegin,
            divEnd,
            divItem,
            getTags_("div"),
            divBuilder.build()
        );
    }

}


LayerTagCollection NKJPLatticeReader::Worker::getTags_(std::string mainTag) {
    return lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("nkjp-reader")(mainTag.c_str())
    );
}


Lattice::EdgeDescriptor NKJPLatticeReader::Worker::appendSegmentToLattice_(
    std::string segment,
    AnnotationItem item,
    std::string mainTag,
    bool insertSpace
) {
    if (insertSpace) {
        Lattice::VertexDescriptor sFrom = lattice_.getLastVertex();
        lattice_.appendStringWithSymbols(" ");
        Lattice::VertexDescriptor sTo = lattice_.getLastVertex();
        std::string blank(" ");
        AnnotationItem sItem("B", blank);
        Lattice::EdgeSequence::Builder sBuilder(lattice_);
        sBuilder.addEdge(lattice_.firstOutEdge(
            sFrom,
            lattice_.getLayerTagManager().getMask("symbol")
        ));
        lattice_.addEdge(sFrom, sTo, sItem, getTags_(mainTag), sBuilder.build());
    }

    Lattice::VertexDescriptor from = lattice_.getLastVertex();
    lattice_.appendStringWithSymbols(segment);
    Lattice::VertexDescriptor to = lattice_.getLastVertex();
    Lattice::EdgeSequence::Builder seqBuilder(lattice_);
    Lattice::VertexDescriptor currentVertex = from;
    Lattice::EdgeDescriptor currentEdge;
    while (currentVertex != to) {
        currentEdge = lattice_.firstOutEdge(
            currentVertex,
            lattice_.getLayerTagManager().getMask("symbol")
        );
        seqBuilder.addEdge(currentEdge);
        currentVertex = lattice_.getEdgeTarget(currentEdge);
    }
    return lattice_.addEdge(from, to, item, getTags_(mainTag), seqBuilder.build());
}

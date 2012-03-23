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
    const boost::program_options::variables_map& options
) {
    return new NKJPLatticeReader(
        options["layer"].as<std::string>()
    );
}

boost::program_options::options_description NKJPLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("layer", boost::program_options::value<std::string>()->default_value("morphosyntax"),
            "specifies the annotation layer represented by the given NKJP file")
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

    switch (processor_.getLayer()) {
        case TEXT :

    {

    BOOST_FOREACH(
        boost::property_tree::ptree::value_type &vDiv,
        xpt.get_child("teiCorpus.TEI.text.body")
    ) if (strcmp(vDiv.first.data(), "<xmlattr>") != 0) {
        Lattice::VertexDescriptor divBegin = lattice_.getLastVertex();
        Lattice::EdgeSequence::Builder divBuilder(lattice_);
        BOOST_FOREACH(
            boost::property_tree::ptree::value_type &vAB,
            vDiv.second.get_child("")
        ) if (strcmp(vAB.first.data(), "<xmlattr>") != 0) {
            std::string abText(vAB.second.get<std::string>(""));
            AnnotationItem abItem(vAB.first.data(), abText);
            Lattice::EdgeDescriptor abEdge = appendSegmentToLattice_(abText, abItem, "paragraph");
            divBuilder.addEdge(abEdge);
        }
        Lattice::VertexDescriptor divEnd = lattice_.getLastVertex();
        AnnotationItem divItem(
            vDiv.first.data(),
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

    } // end of case block (text)

        break;
        case ANN_SEGMENTATION :

    {

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
            ) {
                if (strcmp(vSeg.first.data(), "seg") == 0) {

                    bool insertSpace = false;
                    try {
                        std::string corresp(vSeg.second.get<std::string>("<xmlattr>.corresp"));
                        NKJPSegmentationCorrespGrammar grammar;
                        NKJPSegmentationCorrespItem item;
                        std::string::const_iterator begin = corresp.begin();
                        std::string::const_iterator end = corresp.end();
                        if (parse(begin, end, grammar, item)) {
                            if (item.beginning - prevEnding > 0) {
                                insertSpace = true;
                            } else {
                                insertSpace = false;
                            }
                            prevEnding = item.beginning + item.length;
                        } else {
                            insertSpace = false;
                        }
                    } catch (boost::property_tree::ptree_error) {
                        insertSpace = false;
                    }

                    AnnotationItem segItem(
                        vSeg.second.get<std::string>("<xmlattr>.xml:id"),
                        StringFrag(segText)
                    );
                    std::string npsValue = vSeg.second.get("<xmlattr>.nkjp:nps", "");
                    if (!npsValue.empty()) {
                        lattice_.getAnnotationItemManager().setValue(segItem, "nps", npsValue);
                    }
                    Lattice::EdgeDescriptor segEdge
                        = appendSegmentToLattice_(segText, segItem, "token", insertSpace);
                    sBuilder.addEdge(segEdge);
                    sEnd = lattice_.getEdgeTarget(segEdge);

                } else if (strcmp(vSeg.first.data(), "<xmlcomment>") == 0) {
                    segText = boost::algorithm::trim_copy(vSeg.second.get<std::string>(""));
                }
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
    }

    } // end of case block (segmentation)

        break;
        case ANN_SENSES :

        // TODO

        break;
        case ANN_WORDS :

        // TODO

        break;
        case ANN_NAMED :

        // TODO

        break;
        case ANN_GROUPS :

        // TODO

        break;
        case ANN_MORPHOSYNTAX :
        default:

    {

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
        Lattice::EdgeSequence::Builder pBuilder(lattice_);
        BOOST_FOREACH(
            boost::property_tree::ptree::value_type &vS,
            vP.second.get_child("")
        ) if (strcmp(vS.first.data(), "s") == 0) {
            Lattice::VertexDescriptor sBegin = lattice_.getLastVertex();
            Lattice::EdgeSequence::Builder sBuilder(lattice_);
            BOOST_FOREACH(
                boost::property_tree::ptree::value_type &vSeg,
                vS.second.get_child("")
            ) if (strcmp(vSeg.first.data(), "seg") == 0) {
                bool insertSpace = false;
                try {
                    std::string comment(vSeg.second.get<std::string>("fs.<xmlcomment>"));
                    NKJPMorphosyntaxCommentGrammar grammar;
                    NKJPMorphosyntaxCommentItem item;
                    std::string::const_iterator begin = comment.begin();
                    std::string::const_iterator end = comment.end();
                    if (parse(begin, end, grammar, item)) {
                        if (item.beginning - prevEnding > 0) {
                            insertSpace = true;
                        } else {
                            insertSpace = false;
                        }
                        prevEnding = item.beginning + item.length;
                    } else {
                        insertSpace = false;
                    }
                } catch (boost::property_tree::ptree_error) {
                    insertSpace = false;
                }
                try {
                    std::string segment(vSeg.second.get("fs.f.string", std::string()));
                    std::string npsValue;
                    std::string disambId;
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vF,
                        vSeg.second.get_child("fs")
                    ) if (strcmp(vF.first.data(), "f") == 0) {
                        if (vF.second.get<std::string>("<xmlattr>.name")=="orth") {
                            segment = vF.second.get<std::string>("string");
                        } else if (vF.second.get<std::string>("<xmlattr>.name")=="nps") {
                            npsValue = vF.second.get<std::string>("binary.<xmlattr>.value");
                        } else if (vF.second.get<std::string>("<xmlattr>.name")=="disamb") {
                            BOOST_FOREACH(
                                boost::property_tree::ptree::value_type &vFF,
                                vF.second.get_child("fs")
                            ) if (strcmp(vFF.first.data(), "f") == 0) {
                                if (vFF.second.get<std::string>("<xmlattr>.name")=="choice") {
                                    disambId
                                        = vFF.second.get<std::string>("<xmlattr>.fVal").substr(1);
                                    // substr(1) cuts off the first "#" character
                                }
                            }
                        }
                    }
                    AnnotationItem item("seg", segment);
                    if (!npsValue.empty()) {
                        lattice_.getAnnotationItemManager().setValue(item, "nps", npsValue);
                    }
                    Lattice::EdgeDescriptor segEdge
                        = appendSegmentToLattice_(segment, item, "token", insertSpace);
                    Lattice::VertexDescriptor segBegin = lattice_.getEdgeSource(segEdge);
                    Lattice::VertexDescriptor segEnd = lattice_.getEdgeTarget(segEdge);
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vF,
                        vSeg.second.get_child("fs")
                    ) if (strcmp(vF.first.data(), "f") == 0) {
                        if (vF.second.get<std::string>("<xmlattr>.name")=="interps") {
                            BOOST_FOREACH(
                                boost::property_tree::ptree::value_type &vFS,
                                vF.second.get_child("")
                            ) if (strcmp(vFS.first.data(), "fs") == 0) {
                                std::string base(vF.second.get("f.string", std::string()));
                                BOOST_FOREACH(
                                    boost::property_tree::ptree::value_type &vFF,
                                    vFS.second.get_child("")
                                ) if (strcmp(vFF.first.data(), "f") == 0) {
                                    if (vFF.second.get<std::string>("<xmlattr>.name")=="base") {
                                        base = vFF.second.get<std::string>("string");
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
                                    vFS.second.get_child("")
                                ) if (strcmp(vFF.first.data(), "f") == 0) {
                                    if (vFF.second.get<std::string>("<xmlattr>.name")=="ctag") {
                                        Lattice::EdgeSequence::Builder ctagBuilder(lattice_);
                                        ctagBuilder.addEdge(baseEdge);
                                        AnnotationItem ctagItem(
                                            vFF.second.get<std::string>("symbol.<xmlattr>.value"),
                                            StringFrag(
                                                base + "+" + vFF.second.get<std::string>(
                                                    "symbol.<xmlattr>.value")
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
                                        vFF.second.get<std::string>("<xmlattr>.name")=="msd"
                                    ) {
                                        try {
                                            Lattice::EdgeSequence::Builder msdBuilder(lattice_);
                                            msdBuilder.addEdge(baseEdge);
                                            AnnotationItem msdItem(
                                                vFF.second.get<std::string>(
                                                    "symbol.<xmlattr>.xml:id"),
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
                                                    "symbol.<xmlattr>.value")
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
                                                    vVAlt.second.get<std::string>(
                                                        "<xmlattr>.xml:id"),
                                                    StringFrag(
                                                        lattice_.getAllText(),
                                                        segBegin,
                                                        segEnd-segBegin
                                                    )
                                                );
                                                lattice_.getAnnotationItemManager().setValue(
                                                    vAltItem,
                                                    "value",
                                                    vVAlt.second.get<std::string>("<xmlattr>.value")
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
                                                    disambId != vVAlt.second.get<std::string>(
                                                        "<xmlattr>.xml:id"
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
                    sBuilder.addEdge(segEdge);
                } catch (boost::property_tree::ptree_error) {
                    throw PsiException("nkjp-reader: input file error");
                }
            }
            Lattice::VertexDescriptor sEnd = lattice_.getLastVertex();
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
        }
        Lattice::VertexDescriptor pEnd = lattice_.getLastVertex();
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
    }

    } // end of case block (morphosyntax/default)

    } // end of switch statement

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

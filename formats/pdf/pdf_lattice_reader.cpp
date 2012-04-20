#include "pdf_lattice_reader.hpp"

#include <sstream>

#include <boost/assign/list_of.hpp>

#include "plugin_manager.hpp"


PDFLatticeReader::PDFLatticeReader() {
    adapter_ = dynamic_cast<PopplerAdapterInterface*>(
        PluginManager::getInstance().createPluginAdapter("poppler")
    );
}

PDFLatticeReader::~PDFLatticeReader() {
    if (adapter_) {
        PluginManager::getInstance().destroyPluginAdapter("poppler", adapter_);
    }
}

std::string PDFLatticeReader::getFormatName() {
    return "PDF";
}

std::string PDFLatticeReader::doInfo() {
    return "PDF reader";
}

PopplerAdapterInterface * PDFLatticeReader::getAdapter() {
    return adapter_;
}

bool PDFLatticeReader::isActive() {
    if (adapter_) {
        return true;
    } else {
        return false;
    }
}


PDFLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* PDFLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new PDFLatticeReader();
}

boost::program_options::options_description PDFLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string PDFLatticeReader::Factory::doGetName() const {
    return "pdf-reader";
}

boost::filesystem::path PDFLatticeReader::Factory::doGetFile() const {
    return __FILE__;
}

PDFLatticeReader::Worker::Worker(
    PDFLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("pdf-reader")))
{ }

void PDFLatticeReader::Worker::doRun() {
    if (processor_.isActive()) {
        std::stringstream textStream;
        processor_.getAdapter()->convertPDFToText(inputStream_, textStream);
        std::string line;
        while (getline(textStream, line)) {
            appendParagraphToLattice_(line, textTags_);
            lattice_.appendString("\n");
        }
    }
}

#include "djvu_lattice_reader.hpp"

#include <sstream>

#include <boost/assign/list_of.hpp>

#include "plugin_manager.hpp"


DjVuLatticeReader::DjVuLatticeReader() {
    adapter_ = dynamic_cast<DjVuAdapterInterface*>(
        PluginManager::getInstance().createPluginAdapter("djvu")
    );
}

DjVuLatticeReader::~DjVuLatticeReader() {
    if (adapter_) {
        PluginManager::getInstance().destroyPluginAdapter("djvu", adapter_);
    }
}

std::string DjVuLatticeReader::getFormatName() {
    return "DjVu";
}

std::string DjVuLatticeReader::doInfo() {
    return "DjVu reader";
}

DjVuAdapterInterface * DjVuLatticeReader::getAdapter() {
    return adapter_;
}

bool DjVuLatticeReader::isActive() {
    if (adapter_) {
        return true;
    } else {
        return false;
    }
}


DjVuLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* DjVuLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new DjVuLatticeReader();
}

boost::program_options::options_description DjVuLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string DjVuLatticeReader::Factory::doGetName() const {
    return "djvu-reader";
}

boost::filesystem::path DjVuLatticeReader::Factory::doGetFile() const {
    return __FILE__;
}


DjVuLatticeReader::Worker::Worker(
    DjVuLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("djvu-reader")))
{ }


void DjVuLatticeReader::Worker::doRun() {
    if (processor_.isActive()) {
        std::stringstream textStream;
        processor_.getAdapter()->convertDjVuToText(inputStream_, textStream);
        std::string line;
        while (getline(textStream, line)) {
            appendParagraphToLattice_(line, textTags_);
            lattice_.appendString("\n");
        }
    }
}

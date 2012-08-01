#include "doc_lattice_reader.hpp"

#include <cstdio>
#include <cstdlib>

#include <boost/assign/list_of.hpp>

#include "plugin_manager.hpp"

#include "logging.hpp"


DocLatticeReader::DocLatticeReader() {
    adapter_ = dynamic_cast<AntiwordAdapterInterface*>(
        PluginManager::getInstance().createPluginAdapter("antiword")
    );
}

DocLatticeReader::~DocLatticeReader() {
    if (adapter_) {
        PluginManager::getInstance().destroyPluginAdapter("antiword", adapter_);
    }
}

std::string DocLatticeReader::getFormatName() {
    return "Doc";
}

std::string DocLatticeReader::doInfo() {
    return "Doc reader";
}

AntiwordAdapterInterface * DocLatticeReader::getAdapter() {
    return adapter_;
}

bool DocLatticeReader::isActive() {
    if (adapter_) {
        return true;
    } else {
        return false;
    }
}


DocLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* DocLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new DocLatticeReader();
}

boost::program_options::options_description DocLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options();

    return optionsDescription;
}

std::string DocLatticeReader::Factory::doGetName() const {
    return "doc-reader";
}

boost::filesystem::path DocLatticeReader::Factory::doGetFile() const {
    return __FILE__;
}

bool DocLatticeReader::Factory::doCheckRequirements(
    const boost::program_options::variables_map& options,
    std::ostream & message
) const {
    return PluginManager::getInstance().checkPluginRequirements("antiword", options, message);
}


DocLatticeReader::Worker::Worker(
    DocLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("doc-reader")))
{ }


void DocLatticeReader::Worker::doRun() {

    if (processor_.isActive()) {

        char * tmpFileIn = tempnam(NULL, "psii_");
        char * tmpFileOut = tempnam(NULL, "psio_");
        std::string tmpFileInName(tmpFileIn);
        std::string tmpFileOutName(tmpFileOut);

        std::ofstream strIn(tmpFileIn);
        strIn << inputStream_.rdbuf() << std::flush;
        processor_.getAdapter()->convertWordToText(tmpFileInName, tmpFileOutName);

        std::ifstream strOut(tmpFileOut);
        std::string line;
        while (std::getline(strOut, line)) {
            appendParagraphToLattice_(line, textTags_);
            lattice_.appendString("\n");
        }

        std::remove(tmpFileIn);
        std::remove(tmpFileOut);
        free(tmpFileIn);
        free(tmpFileOut);

    }

}

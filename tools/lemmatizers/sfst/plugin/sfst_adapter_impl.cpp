#include "sfst_adapter_impl.hpp"

#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "logging.hpp"

SfstAdapterImpl::SfstAdapterImpl() {
}

SfstAdapterImpl::~SfstAdapterImpl() {
}

void SfstAdapterImpl::initSfst(FILE* file) {
    transducer_.reset(new SFST::Transducer(file));
}

bool SfstAdapterImpl::analyzeString(const std::string& word, FILE* outputFile) {
    return transducer_->analyze_string((char*)word.c_str(), outputFile, true);
}

// ==============================================

extern "C" SfstAdapterImpl* create() {
    return new SfstAdapterImpl;
}

extern "C" void destroy(SfstAdapterImpl* Tl) {
    delete Tl;
}

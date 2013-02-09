#include "hfst_adapter_impl.hpp"

HfstAdapterImpl::HfstAdapterImpl() {
}

HfstAdapterImpl::~HfstAdapterImpl() {
}

void HfstAdapterImpl::initHfst(const std::string &file) {
    hfst::HfstInputStream transducerStream(file); //@todo error handling
    //or: hfstTransducer_ = boost::scoped_ptr<hfst::HfstTransducer>(new hfst::HfstTransducer(transducerStream));
    hfstTransducer_.reset(new hfst::HfstTransducer(transducerStream));
}

bool comparePaths_(const HfstPath &a, const HfstPath &b) {
    if (a.weight != b.weight)
        return (a.weight < b.weight);
    else
        return true;
}

HfstPaths HfstAdapterImpl::analyzeWord(const std::string& word) {
    HfstPaths paths;
    hfst::HfstOneLevelPaths *hfstPaths = hfstLookup_(word);
    for (hfst::HfstOneLevelPaths::iterator pathIt = hfstPaths->begin();
            pathIt != hfstPaths->end(); ++ pathIt) {
        HfstPath path;
        path.weight = pathIt->first;
        path.path = pathIt->second;
        paths.push_back(path);
    }
    paths.sort(comparePaths_);
    return paths;
}

hfst::HfstOneLevelPaths* HfstAdapterImpl::hfstLookup_(const std::string& word) {
    return hfstTransducer_->lookup(word);
}


extern "C" HfstAdapterImpl* create() {
    return new HfstAdapterImpl;
}

extern "C" void destroy(HfstAdapterImpl* Tl) {
    delete Tl;
}

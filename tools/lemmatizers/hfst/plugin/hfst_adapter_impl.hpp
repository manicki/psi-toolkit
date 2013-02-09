#ifndef HFST_ADAPTER_IMPL_HDR
#define HFST_ADAPTER_IMPL_HDR

#include "hfst_adapter_interface.hpp"
#include <boost/scoped_ptr.hpp>
#include <HfstTransducer.h>

class HfstAdapterImpl : public HfstAdapterInterface {
public:
    HfstAdapterImpl();
    virtual ~HfstAdapterImpl();

    virtual void initHfst(const std::string& filename);
    virtual HfstPaths analyzeWord(const std::string &word);
private:
    hfst::HfstOneLevelPaths* hfstLookup_(const std::string& word);
    boost::scoped_ptr<hfst::HfstTransducer> hfstTransducer_;
};

#endif

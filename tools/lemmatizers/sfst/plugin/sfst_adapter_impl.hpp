#ifndef SFST_ADAPTER_IMPL_HDR
#define SFST_ADAPTER_IMPL_HDR

#include <boost/scoped_ptr.hpp>

#include "sfst_adapter_interface.hpp"

#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H 0
#endif

#ifndef HAVE_BACKWARD_HASH_MAP
#define HAVE_BACKWARD_HASH_MAP 0
#endif

#ifndef HAVE_HASH_MAP
#define HAVE_HASH_MAP 0
#endif

#ifndef HAVE_EXT_HASH_MAP
#define HAVE_EXT_HASH_MAP 0
#endif

#ifndef HAVE_BACKWARD_HASH_SET
#define HAVE_BACKWARD_HASH_SET 0
#endif

#ifndef HAVE_EXT_HASH_SET
#define HAVE_EXT_HASH_SET 0
#endif

#ifndef HAVE_HASH_SET
#define HAVE_HASH_SET 0
#endif

#ifndef SGIext
#define SGIext 0
#endif

#ifndef SGI__gnu_cxx
#define SGI__gnu_cxx 0
#endif

#include <fst.h>

class SfstAdapterImpl : public SfstAdapterInterface {
public:
    SfstAdapterImpl();
    virtual ~SfstAdapterImpl();

    virtual void initSfst(FILE* file);
    virtual bool analyzeString(const std::string& word, FILE* outputFile);

private:
    boost::scoped_ptr<SFST::Transducer> transducer_;
};

#endif

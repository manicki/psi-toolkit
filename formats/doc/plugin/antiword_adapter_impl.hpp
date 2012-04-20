#ifndef ANTIWORD_ADAPTER_IMPL_HDR
#define ANTIWORD_ADAPTER_IMPL_HDR

#include "antiword_adapter_interface.hpp"


class AntiwordAdapterImpl : public AntiwordAdapterInterface {
public:
    AntiwordAdapterImpl() { }

    virtual ~AntiwordAdapterImpl() { }

    virtual void convertWordToText(std::string sourceFileName, std::string targetFileName);
};


#endif

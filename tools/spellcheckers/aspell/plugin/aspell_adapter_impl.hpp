#ifndef ASPELL_ADAPTER_IMPL_HDR
#define ASPELL_ADAPTER_IMPL_HDR

#include "aspell_adapter_interface.hpp"
#include <aspell.h>

class AspellAdapterImpl : public AspellAdapterInterface {
public:
    AspellAdapterImpl();
    virtual ~AspellAdapterImpl();

    virtual bool isWordCorrect(const std::string & word);

    virtual void getSuggestionsForLastWord(
                                   SuggestionsList & suggestionsList,
                                   const std::string & word
                                   );

    virtual void initAspell(const std::string & langCode);
    virtual void passOptionsToAspellConfig(
         const boost::program_options::variables_map& options);
    virtual void createAspellInstance();

private:
    AspellConfig * aspellConfig_;
    AspellSpeller * aspellSpeller_;

    unsigned int limitCandidates_;
    std::string langCode_;

};

#endif

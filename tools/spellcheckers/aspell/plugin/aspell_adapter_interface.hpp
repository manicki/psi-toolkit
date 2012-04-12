#ifndef ASPELL_ADAPTER_INTERFACE_HDR
#define ASPELL_ADAPTER_INTERFACE_HDR

#include <string>
#include <boost/program_options.hpp>
#include <list>

typedef std::list<std::string> SuggestionsList;

#include "plugin_adapter.hpp"

class AspellAdapterInterface : public PluginAdapter {
public:
    virtual ~AspellAdapterInterface() {
    }

    virtual bool isWordCorrect(const std::string & word) = 0;
    virtual void getSuggestionsForLastWord(
                 SuggestionsList & suggestionsList,
                 const std::string & word
                                           ) = 0;

    virtual void initAspell(const std::string & langCode) = 0;
    virtual void passOptionsToAspellConfig(
         const boost::program_options::variables_map& options) = 0;
    virtual void createAspellInstance() = 0;
};

#endif

#ifndef FAKE_LEMMATIZER_HDR
#define FAKE_LEMMATIZER_HDR

#include <string>
#include <list>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"

class FakeLemmatizer {

public:
    FakeLemmatizer(const boost::program_options::variables_map& options);

    bool lemmatize(const std::string& token,
                   AnnotationItemManager& annotationItemManager,
                   LemmatizerOutputIterator& outputIterator);

    static boost::program_options::options_description optionsHandled();

    static std::string getName();

    std::list<std::string> getLayerTags();

    std::string getLanguage() const;
};

#endif

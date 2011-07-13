#ifndef LAYER_TAG_MANAGER_HDR
#define LAYER_TAG_MANAGER_HDR

#include <list>
#include <string>

#include <boost/bimap.hpp>

#include "layer_tag_collection.hpp"

/*!
  LayerTagManager is used to
  * create layer tags
  * keep mapping to string so that LayerTagCollection is just a bit vector
  */
class LayerTagManager {

public:
    LayerTagManager();

    LayerTagCollection createSingletonTagCollection(std::string tag_name);

    LayerTagCollection createTagCollection(std::list<std::string> tag_names);

    /**
     * returns tags sorted alphabetically
     */
    std::list<std::string> getTagNames(const LayerTagCollection& tag_collection);

private:
    typedef boost::bimap<std::string,int> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;
    
    int size_;

};

#endif

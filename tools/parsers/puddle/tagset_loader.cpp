#include "tagset_loader.hpp"

namespace poleng {

namespace bonsai {
namespace puddle {

    TagsetPtr TagsetLoader::load(std::string filename) {
        TagsetPtr tagset = TagsetPtr(new Tagset());
        tagset->readFromFile(filename);
        return tagset;
    }

#if _WITH_BONSAI_PARSEGRAPH
    TagsetPtr TagsetLoader::load(std::string filename, std::string desc_filename) {
        TagsetPtr tagset = TagsetPtr(new Tagset());
        tagset->readFromFile(filename);
        tagset->readDescFromFile(desc_filename);
        return tagset;
    }
#endif

}

}

}

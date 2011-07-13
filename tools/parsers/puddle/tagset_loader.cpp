
#include "tagset_loader.hpp"

namespace poleng
{

namespace bonsai
{
namespace puddle
{

    TagsetLoader::TagsetLoader()
    {
    }

    TagsetLoader::~TagsetLoader()
    {
    }

    TagsetPtr TagsetLoader::load(std::string filename)
    {
        TagsetPtr tagset = TagsetPtr(new Tagset());
        tagset->readFromFile(filename);
        return tagset;
    }

    TagsetPtr TagsetLoader::load(std::string filename, std::string desc_filename)
    {
        TagsetPtr tagset = TagsetPtr(new Tagset());
        tagset->readFromFile(filename);
        tagset->readDescFromFile(desc_filename);
        return tagset;
    }

}

}

}

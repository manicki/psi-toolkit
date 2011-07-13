#ifndef TAGSETLOADER_H__
#define TAGSETLOADER_H__

#include <string>
#include "tagset.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {
        class TagsetLoader
        {
            public:
                TagsetLoader();
                ~TagsetLoader();
                TagsetPtr load(std::string filename);
                TagsetPtr load(std::string filename, std::string desc_filename);
            private:
        };
    }
}

}

#endif


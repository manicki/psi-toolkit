#ifndef TAGSETLOADER_HDR
#define TAGSETLOADER_HDR

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
                TagsetLoader() {}
                TagsetPtr load(std::string filename);
#if _WITH_BONSAI_PARSEGRAPH
                TagsetPtr load(std::string filename, std::string desc_filename);
#endif
            private:
        };
    }
}

}

#endif


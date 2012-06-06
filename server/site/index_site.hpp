#include <string>

#include "template_site.hpp"

class IndexSite : public TemplateSite
{

public:
    IndexSite(PsiServer& server);

    char * scriptsPart();
    char * headerPart();
    char * promotionPart();
    char * footerPart();
};

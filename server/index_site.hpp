#include <string>

#include "template_site.hpp"

class IndexSite : public TemplateSite
{

public:
	std::string inputText;

	IndexSite(PsiServer& server);

	char * info();
    char * scriptsPart();
    char * headerPart();
	char * footerPart();

};

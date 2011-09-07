#include <string>
#include <boost/bind.hpp>

#include "psi_server.hpp"

class TemplateSite{

public:

	TemplateSite(PsiServer& server) : psi_server_(server) {};
	
protected:

	PsiServer& psi_server_;

	char * StringToChar(std::string str) {
		char * chr = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), chr);
		chr[str.size()] = '\0';

		return chr;
	}

};

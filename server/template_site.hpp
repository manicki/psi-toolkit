#ifndef TEMPLATE_SITE_HDR
#define TEMPLATE_SITE_HDR

#include <fstream>
#include <iostream>

#include <string>
#include <boost/bind.hpp>

#include "psi_server.hpp"

class TemplateSite{

public:

	TemplateSite(PsiServer& server) : psi_server_(server) {};
	
protected:

	PsiServer& psi_server_;

	char * stringToChar(std::string str) {
		char * chr = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), chr);
		chr[str.size()] = '\0';

		return chr;
	}

	std::string readPsisFile(const char * fileName) {
		std::string content = "";
		std::string line;

		std::string filePath = psi_server_.websiteRoot;
		filePath += "/"; 
		filePath += fileName;

		std::ifstream myfile(filePath.c_str());

  		if (myfile.is_open()) {
			while (myfile.good()) {
      			std::getline(myfile, line);
      			content += line + "\n";
    		}
    		myfile.close();
  		}
		
		return content;
	}
};

#endif

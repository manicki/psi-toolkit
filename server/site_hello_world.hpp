#include <string>
#include <boost/bind.hpp>

#include "psi_server.hpp"

class SiteHelloWorld{
	std::string userName;
	PsiServer& psi_server_;

	public:
	SiteHelloWorld(PsiServer& server) : psi_server_(server) {
		psi_server_.registerIncludeCode("hello", boost::bind(&SiteHelloWorld::hello, this));
		psi_server_.registerIncludeCode("hellouser", boost::bind(&SiteHelloWorld::helloUser, this));
		psi_server_.registerActionCode("name", boost::bind(&SiteHelloWorld::actionHello, this));
	}
	
	char * hello() {
		return StringToChar(std::string("<em>Hello World from class! <br /> Zażółć gęślą jaźń!</em>"));
	}

	char * helloUser() {
		std::string str = "";
		if (userName.length()) {
			str = "Hello, <em>" + userName + "</em>";
		}

		return StringToChar(str);
	}

	char * actionHello() {
		userName = psi_server_.findValue("yourname");
		return StringToChar(std::string("/index.html"));
	}

	private:
	char * StringToChar(std::string str) {
		char * chr = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), chr);
		chr[str.size()] = '\0';

		return chr;
	}

};

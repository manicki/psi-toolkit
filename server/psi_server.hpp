#ifndef PSI_SERVER_HDR
#define PSI_SERVER_HDR

#pragma once

#include <string>
#include <map>
#include <boost/function.hpp>
#include "server/server.hpp"
#include "psi_request_handler.hpp"

#if !defined(_WIN32)

#include <pthread.h>
#include <signal.h>

	class PsiServer;
	typedef boost::function<char*()> psis_include_function;
	typedef boost::function<char*(PsiServer*)> psis_action_function;

	class PsiServer{
	
	public:
		PsiServer(const std::string& address, const std::string& port,
			const std::string& threads, const std::string& doc_root);

		void run();

		std::string& info();

		void registerIncludeCode(const char* idname, psis_include_function fun);
		void registerActionCode(const char* idname, psis_action_function fun);
		void include(std::string& reply);

		void checkForAction(http::server3::request& req);
		std::string& findValue(const char* name);

		std::string websiteRoot;

	//private:
		std::string urlDecode(std::string &);		

		/// store map between include codes and application functions
		std::map <std::string, psis_include_function > includes_;
		/// store map between action codes and application functions
		std::map <std::string, psis_action_function > actions_;
		/// store name value pairs for form submit action
		std::multimap <std::string, std::string> name_values_;

		// port server is listening on
		std::string port_;
		// request handler specialized to handle PsiServer requests
		PsiRequestHandler psi_request_handler_;
		// boost::asio web server
		http::server3::server server_;
	};

#endif // !defined(_WIN32)
#endif
